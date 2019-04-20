#include <iostream>
#include <vector>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <queue>
using namespace std;

int main()
{
#if 0
	FILE *pf = fopen("data.dat", "wb");
	for (int i = 0; i < 100000; ++i)
	{
		int data = rand() % 65535;
		fwrite(&data, 4, 1, pf);
	}
	fclose(pf);
#endif
	//  求data.dat文件中重复次数最高的前5个   11个小文件data
	FILE *pf = fopen("data.dat", "rb");
	FILE *pfArr[11];
	char buf[20];
	for (int i = 0; i < 11; ++i)
	{
		sprintf(buf, "data%d.dat", i + 1);
		pfArr[i] = fopen(buf, "wb+");
	}
	int data = 0;
	while (!feof(pf))
	{
		fread(&data, 4, 1, pf);
		int index = data % 11;
		if (data < 0)
		{
			cout << endl;
		}
		fwrite(&data, 4, 1, pfArr[index]);
	}
	fclose(pf);

	// 
	unordered_map<int, int> map;
	using P = pair<int, int>;
	using FUNC = function<bool(P&, P&)>;
	using MinHeap = priority_queue<P, vector<P>, FUNC>;
	MinHeap minHeap([](auto &a, auto &b)->bool
	{
		return a.second > b.second;
	});

	for (int i = 0; i < 11; ++i)
	{
		int length = ftell(pfArr[i]);
		int size = length / sizeof(int);
		fseek(pfArr[i], 0, SEEK_SET);

		int *pbuf = new int[size];
		fread(pbuf, length, 1, pfArr[i]);
		map.clear();
		for (int j = 0; j < size; ++j)
		{
			if (pbuf[j] < 0)
			{
				cout << endl;
			}
			map[pbuf[j]]++;
		}
		delete[]pbuf;
		fclose(pfArr[i]);

		// 求当前文件数据的top k
		for_each(map.begin(),
			map.end(),
			[&minHeap](auto &pair)->void
		{
			if (minHeap.size() < 5)
			{
				minHeap.push(pair);
			}
			else
			{
				if (pair.second > minHeap.top().second)
				{
					minHeap.pop();
					minHeap.push(pair);
				}
			}
		});
	}

	while (!minHeap.empty())
	{
		cout << "数字：" << minHeap.top().first << " 重复次数:"
			<< minHeap.top().second << endl;
		minHeap.pop();
	}
	cout << endl;
	return 0;
}