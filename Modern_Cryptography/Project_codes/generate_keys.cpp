// compile it: g++ -o generate_keys generate_kets.cpp -lssl -lcrypto
#include <iostream>
#include <string>
#include <fstream>

#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>

using namespace std;

void show_key_rsa() {
	// read public_key.pem
	fstream file1 ("public_key.pem");
	string readfile = "";
	string tmp = "";
	while (getline(file1, tmp)) {
		readfile += tmp;
		readfile += "\n";
	}
	file1.close();
	cout << "Read public_key.pem: " << endl << readfile << endl;

	// read private_key.pem
	fstream file2 ("private_key.pem");
	readfile = "";
	tmp = "";
	while (getline(file2, tmp)) {
		readfile += tmp;
		readfile += "\n";
	}
	file2.close();
	cout << "Read public_key.pem: " << endl << readfile << endl;
}

void gen_key_rsa() {
	// initialize
	EVP_PKEY* pkey = NULL;
	EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
	EVP_PKEY_keygen_init(ctx);

	// length of the desired keys, and generate keys
	EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 2048);
	EVP_PKEY_keygen(ctx, &pkey);
	EVP_PKEY_CTX_free(ctx);

	// write keys
	BIO* pub_bio = BIO_new_file("public_key.pem", "wb");
	BIO* priv_bio = BIO_new_file("private_key.pem", "wb");
	PEM_write_bio_PUBKEY(pub_bio, pkey);
	PEM_write_bio_PrivateKey(priv_bio, pkey, NULL, NULL, 0, NULL, NULL);
	BIO_free(pub_bio);
	BIO_free(priv_bio);
}

int main() {
	// generate random key pairs - pub_key & priv_key.
	// do it first if you don't have the key pairs to prepare for the encryption.
	gen_key_rsa();
	show_key_rsa();
	
	return 0;
}
