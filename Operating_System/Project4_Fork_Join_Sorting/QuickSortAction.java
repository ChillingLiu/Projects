import java.util.Arrays; //Used for array operations.
import java.util.Random; //Used for random array generating.
import java.util.concurrent.RecursiveAction;
import java.util.concurrent.ForkJoinPool;

class QuickSortAction extends RecursiveAction{
	//Necessary data for processing the quick sort.
	private int[] arr;
	private int low;
	private int high;

	//Initialization.
	public QuickSortAction(int[] arr,int low,int high) {
		this.arr = arr;
		this.high = high;
		this.low = low;
	}

	//Simple algorithm for quick sort.
	@Override
	public void compute() {
		if(low < high){
			int i = low, j = high, base = arr[low];
			while (i < j) {
				while (arr[j] >= base && i < j) {
					j--;
				}
				while (arr[i] <= base && i < j) {
					i++;
				}
				swap(arr, i, j);
			}
			swap(arr, low, j);

			//Fork-Join for quick sort.
			QuickSortAction leftTask =new QuickSortAction(arr, low, j-1);
			QuickSortAction rightTask =new QuickSortAction(arr, j+1, high);
			leftTask.fork();
			rightTask.fork();
			leftTask.join();
			leftTask.join();
		}
	}

	private void swap(int[] arr, int i, int j) {
		int tmp = arr[i];
		arr[i] = arr[j];
		arr[j] = tmp;
	}

	//Generate random array.
	private static int[] buildRandomIntArray(final int size) {
		int[] array = new int[size];
		Random generator = new Random();
		for (int i = 0; i < array.length; i++) {
			//Random value from 0 to 100.
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

		//Start quick sort.
		QuickSortAction quickSortAction = new QuickSortAction(array, 0, array.length - 1);
		pool.invoke(quickSortAction);
		System.out.println("After Fork-Join Quick Sorting:");
		System.out.println(Arrays.toString(array));
		System.out.println("");
    }
}
