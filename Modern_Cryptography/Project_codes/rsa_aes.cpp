// compile: g++ -o rsa_aes rsa_aes.cpp -lssl -lcrypto
#include <iostream>
#include <string>
#include <fstream>

#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/rand.h>

#define AES_KEYLEN 32 // for AES256

using namespace std;

int main(int argc, char** argv) {

	// -------------------- Get Inputs --------------------

	if (argc != 6) {
		cout << "Usage: ./rsa_aes <public-key scheme> <pub_key> <priv_key> <symmetric scheme> <plaintext file>" << endl;
		cout << "For example: ./rsa_aes rsa public_key.pem private_key.pem ase plaintext.txt" << endl;
		return -1;
	}
	char* pk_scheme = argv[1];
	char* pub_key_file = argv[2];
	char* priv_key_file = argv[3];
	char* symmetric_schme = argv[4];
	char* plaintext_file = argv[5];


	// -------------------- Initialzation --------------------
	// read message
	ifstream file (plaintext_file);
	string readfile = "";
	string tmp;
	while (getline(file, tmp)) {
		readfile += tmp;
		readfile += "\n";
	}
	file.close();
	// we must use unsigned char* to fit the functions of openssl.
	unsigned char* plaintext = (unsigned char *)readfile.c_str();
	cout << "Original plaintext: " << plaintext << endl;

	// initialize encrypted ciphertext buffer.
	unsigned char cipher_key[256];
	size_t cipher_key_len = 0;


	// -------------------- Encrypt Symmetric Key Using RSA --------------------


	// read keys
	BIO* pub_bio = BIO_new_file(pub_key_file, "r");
	BIO* priv_bio = BIO_new_file(priv_key_file, "r");
	EVP_PKEY *pub_key = PEM_read_bio_PUBKEY(pub_bio, NULL, NULL, NULL);
	EVP_PKEY *priv_key = PEM_read_bio_PrivateKey(priv_bio, NULL, NULL, NULL);
	BIO_free(pub_bio);
	BIO_free(priv_bio);

	// generate random symmetric keys
	unsigned char key[AES_KEYLEN/8];
	unsigned char iv[AES_KEYLEN/8];
	RAND_bytes(key, sizeof(key));
	RAND_bytes(iv, sizeof(iv));
	cout << "Random symmetric key (AES256): " << endl;
	BIO_dump_fp(stdout, key, sizeof(key));
	cout << "Random initialization vector: " << endl;
	BIO_dump_fp(stdout, iv, sizeof(iv));

	// encrypt symmetric key using public key scheme
	EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(pub_key, NULL);
	EVP_PKEY_encrypt_init(ctx);
	EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_PADDING);
	EVP_PKEY_encrypt(ctx, cipher_key, &cipher_key_len, key, sizeof(key));
	EVP_PKEY_CTX_free(ctx);
	cout << endl << "Ciphert key using RSA is: " << endl;
	BIO_dump_fp(stdout, cipher_key, cipher_key_len);


	// -------------------- Write Encrypted Symmetric Key --------------------


	// write cipher_key - pem_write will be base64 format
	BIO* cipher_key_bio = BIO_new_file("symm.key", "w");
	PEM_write_bio(cipher_key_bio, "DECRYPTED SYMMETRIC KEY", "", cipher_key, cipher_key_len);
	BIO_free(cipher_key_bio);

	// combine cipher_key and plaintext in a tar-ball
	// read cipher_key in base64 format
	ifstream file2 ("symm.key");
	// must use a new stirng, or previous plaintext will be changed
	string readfile2 = "";
	tmp = "";
	while (getline(file2, tmp)) {
		readfile2 += tmp;
		readfile2 += "\n";
	}
	file2.close();
	const char* cipher_key_base64 = (const char *)readfile2.c_str();

	// write cipher_key and plaintext into envelope.env
	FILE* file3 = fopen("envelope.env", "w");
	fprintf(file3, "%s", cipher_key_base64);
	fprintf(file3, "%s", "-----BEGIN MESSAGE-----\n");
	fprintf(file3, "%s", plaintext);
	fprintf(file3, "%s", "-----END MESSAGE-----\n");
	fclose(file3);

	// read envelope_env
	fstream file4 ("envelope.env");
	string readfile3 = "";
	tmp = "";
	while (getline(file4, tmp)) {
		readfile3 += tmp;
		readfile3 += "\n";
	}
	file4.close();
	const unsigned char* envelope_env = (const unsigned char *)readfile3.c_str();
	cout << endl << "Tar-ball envelope.env is: " << endl << envelope_env << endl;
	size_t envelope_env_len = readfile3.length();


	// -------------------- Encrypt envelope (key + message) use AES --------------------


	unsigned char cipher_enc[1024];
	size_t cipher_enc_len;
	int len;

	EVP_CIPHER_CTX *ctx2;
	ctx2 = EVP_CIPHER_CTX_new();

	EVP_EncryptInit_ex(ctx2, EVP_aes_256_cbc(), NULL, key, iv);

	EVP_EncryptUpdate(ctx2, cipher_enc, &len, envelope_env, envelope_env_len);
	cipher_enc_len = len;

	EVP_EncryptFinal_ex(ctx2, cipher_enc + len, &len);
	cipher_enc_len += len;

	EVP_CIPHER_CTX_free(ctx2);

	cout << "After AES_256_cbc symmetric encryption of envelope: " << endl;
	BIO_dump_fp(stdout, cipher_enc, cipher_enc_len);

	// write cipher.enc
	BIO* cipher_enc_bio = BIO_new_file("cipher.enc", "w");
	PEM_write_bio(cipher_enc_bio, "CIPHER ENC", "", cipher_enc, cipher_enc_len);
	BIO_free(cipher_enc_bio);

	// read cipher.enc
	fstream file5 ("cipher.enc");
	string readfile4 = "";
	tmp = "";
	while (getline(file5, tmp)) {
		readfile4 += tmp;
		readfile4 += "\n";
	}
	file5.close();
	cout << endl << "Read cipher.enc: " << endl << readfile4 << endl;


	// -------------------- Decrypt envelope (key + message) --------------------


	unsigned char decryptedtext[1024];
	size_t decrypted_len;
	int len2 = 0;

	EVP_CIPHER_CTX *ctx3;
	ctx3 = EVP_CIPHER_CTX_new();

	EVP_DecryptInit_ex(ctx3, EVP_aes_256_cbc(), NULL, key, iv);

	EVP_DecryptUpdate(ctx3, decryptedtext, &len2, cipher_enc, cipher_enc_len);
	decrypted_len = len2;

	EVP_DecryptFinal_ex(ctx3, decryptedtext + len2, &len2);
	decrypted_len += len2;

	EVP_CIPHER_CTX_free(ctx3);

	cout << endl << "Use AES to decrpt and verify: " << endl;
	cout << decryptedtext << endl;


	// -------------------- Decrypt envelope (key + message) --------------------


	unsigned char decrypted_key[256];
	size_t decrypted_key_len;

	EVP_PKEY_CTX* ctx4 = EVP_PKEY_CTX_new(priv_key, NULL);
	EVP_PKEY_decrypt_init(ctx4);
	EVP_PKEY_decrypt(ctx4, NULL, &decrypted_key_len, cipher_key, cipher_key_len);

	EVP_PKEY_CTX_set_rsa_padding(ctx4, RSA_PKCS1_PADDING);
	EVP_PKEY_decrypt(ctx4, decrypted_key, &decrypted_key_len, cipher_key, cipher_key_len);
	decrypted_key[decrypted_key_len] = '\0';

	EVP_PKEY_CTX_free(ctx4);
	EVP_PKEY_free(pub_key);
	EVP_PKEY_free(priv_key);

	// comparision
	cout << "Decrypted symmetric key using RSA is: " << endl;
	BIO_dump_fp(stdout, decrypted_key, decrypted_key_len);
	cout << "Original random symmetric key: " << endl;
	BIO_dump_fp(stdout, key, sizeof(key));

	return 0;
}
