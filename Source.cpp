#include <thread>
#include <vector>
#include <iostream>
#include <mutex>
using namespace std;

mutex mtx;

void Reader(size_t i, int N, int start)
{
	this_thread::sleep_for(chrono::milliseconds(1000*start));
	for (int j = 0; j < N; j++)
	{
		cout << "ID = " << this_thread::get_id() << "\t" << j << "\t" << i << " Reader" << endl; //j показывает такт i показывает какой поток выполняется

		this_thread::sleep_for(chrono::milliseconds(1000));
	}
}

void Writer(size_t i, int N, int start)
{
	this_thread::sleep_for(chrono::milliseconds(1000 * start));
	mtx.lock();
	for (int j = 0; j < N; j++)
	{
		cout << "ID = " << this_thread::get_id() << "\t" << j << "\t" << i << " Writer" << endl; //j показывает такт i показывает какой поток выполняется

		this_thread::sleep_for(chrono::milliseconds(1000));
	}
	mtx.unlock();
}

int main()
{
	int start[4] = { 0,1,5,6 };
	int duration[4] = { 4,6,2,3 };
	int bb[4] = { 0,0,1,0 };
	vector<thread> ths;
	size_t n;
	cin >> n;
	int start_w = 0;
	int comprasion = 0;

	for (int i = 0; i < n; i++)
	{
		if (bb[i] == 1)
		{
			start_w = start[i];
			comprasion = 0;
			int max_r = 0;
			for (int j = 0; j < i; j++)
			{
				if (bb[j] == 0)
				{
					if ((duration[j]+start[j]) > max_r) max_r = duration[j]+start[j];
				}
				if (bb[j] == 1) comprasion += duration[j];
			}
			comprasion += max_r;
			if (comprasion > start_w) start_w = comprasion;


			for (int j = i; j < n; j++)
			{
				if (start[j] < start_w + duration[i]) start[j] = start_w + duration[i];
			}


			/*for (int j = i; j < n; j++)
			{
				start[j] += start_w;
			}*/
		}
	}

	for (size_t i = 0; i < n; ++i)
		if(bb[i]==0)
			ths.push_back(thread(&Reader, i, duration[i], start[i]));
		else
			ths.push_back(thread(&Writer, i, duration[i], start_w));

	for (auto& th : ths)
	{
		th.join();
	}

	return 0;
}