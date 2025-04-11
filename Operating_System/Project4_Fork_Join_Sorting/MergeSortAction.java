import java.util.Arrays; //Used for array operations.
import java.util.Random; //Used for random array generating.
import java.util.concurrent.RecursiveAction;
import java.util.concurrent.ForkJoinPool;

public class MergeSortAction extends RecursiveAction {
	//Necessary data for processing the merge sort.
	private final int[] arr;

	//Initialization.
	public MergeSortAction(int[] arr) {
		this.arr = arr;
	}

	//Partition first.
	@Override
	public void compute() {
		if (arr.length < 2) return;
		int mid = arr.length / 2;

		int[] left = new int[mid];
		System.arraycopy(arr, 0, left, 0, mid);

		int[] right = new int[arr.length - mid];
		System.arraycopy(arr, mid, right, 0, arr.length - mid);

		//Recursively partition, and use Fork-Join for sorting.
		invokeAll(new MergeSortAction(left), new MergeSortAction(right));
		merge(left, right);
	}

	//Simple algorithm for merge sort.
	private void merge(int[] left, int[] right) {
		int i = 0, j = 0, k = 0;
		while (i < left.length && j < right.length) {
			if (left[i] < right[j])
				arr[k++] = left[i++];
			else
				arr[k++] = right[j++];
		}
		while (i < left.length) {
			arr[k++] = left[i++];
		}
		while (j < right.length) {
			arr[k++] = right[j++];
		}
	}

	//Generate random array.
	private static int[] buildRandomIntArray(final int size) {
		int[] array = new int[size];
		Random generator = new Random();
		for (int i = 0; i < array.length; i++) {
			array[i] = generator.nextInt(100);
		}
		return array;
	}

	//Main function.
	public static void main(String[] args) {
		ForkJoinPool pool = new ForkJoinPool();

		int[] array = buildRandomIntArray(20);
		System.out.println("A random array with length of 20:");
		System.out.println(Arrays.toString(array));

		//Start merge sort.
		MergeSortAction mergeSortAction = new MergeSortAction(array);
		pool.invoke(mergeSortAction);
		System.out.println("After Fork-Join Merge Sorting:");
		System.out.println(Arrays.toString(array));
		System.out.println("");
    }
}
