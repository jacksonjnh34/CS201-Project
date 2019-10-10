#include <iostream>
#include <signal.h>
#include <bits/stdc++.h> 

using namespace std;

template <class elmtype>
class CircularDynamicArray
{
    public:
        CircularDynamicArray()
        {
            data = new elmtype[2];

            head = 0;
            tail = 0;

            size = 0;
            capacity_arr = 2;
        }

        CircularDynamicArray(int s)
        {
            data = new elmtype[s];

            size = s;
            capacity_arr = s;

            head = 0;
            tail = s - 1;
        }

        ~CircularDynamicArray()
        {
            //Ask Dixon if free or delete would be better
            free(data);
            delete(data);

            size = 0;
            capacity_arr = 0;
        }

        elmtype& operator[](int i)
        {
            if(i > (size - 1))
            {
                cout << "Array index out of bounds at [" << i << "]";
                return error;
            }

            return data[(head + i) % capacity_arr];
        }

        CircularDynamicArray& operator=(CircularDynamicArray& other)
        {
            //printf("copying\n");
            data = new elmtype[other.capacity()];
            //printf("array alloced\n");
            capacity_arr = other.capacity();
            //printf("cap set\n");
            size = other.length();
            //printf("size set\n");

            for(int i = 0; i < size; i++)
            {
                //printf("Data: %d\n", other[i]);
                data[i] = other[i];
            }
            //printf("finished\n");

            return *this;
        }

        void addEnd(elmtype v)
        {
            if(capacity_arr == size)
            {
                resize(true);
            }

            data[(head + (size)) % capacity_arr] = v;
            size++;

            tail++;
            if(tail > (capacity_arr - 1))
            {
                tail = 0;
            }
        }

        void addFront(elmtype v)
        {
            if(capacity_arr == size)
            {
                resize(true);
            }

            head--;
            if(abs(head) != head)
            {
                head = capacity_arr - 1;
            }

            data[(head) % capacity_arr] = v;
            size++;
        }

        void delEnd()
        {
            if(size <= capacity_arr / 4)
            {
                resize(false);
            }

            tail--;
            if(abs(tail) != tail)
            {
                tail = capacity_arr - 1;
            }

            size--;
        }
        
        void delFront()
        {
            if(size <= capacity_arr / 4)
            {
                resize(false);
            }

            size--;

            head++;
            if(head > (capacity_arr - 1))
            {
                head = 0;
            }
        }

        int length()
        {
            return size;
        }
        int capacity()
        {
            return capacity_arr;
        }
        void clear()
        {
            this = new CircularDynamicArray();
        }

        elmtype QuickSelect(int k)
        {
            reorder();
            return kSmallest(data, 0, size - 1, k, false, -1);
        }

        elmtype WCSelect(int k)
        {
            reorder();
            int numGroups = (size / 5) + 1;

            int medians[numGroups];

            for(int i = 0; i < numGroups; i++)
            {
                int group[5];
                int num_elem = 0;

                for(int j = 0; j < 5; j++)
                {
                    int idx = (i * 5) + j;

                    if(idx < size)
                    {
                        //cout << data[idx] << endl;

                        group[j] = data[idx];
                        num_elem++;
                    }
                }

                mergeSort(group, 0, num_elem - 1);
                medians[i] = group[(0 + num_elem) / 2];
            }

            //cout << "MEDIANS: ";
            printArray(medians, numGroups);

            elmtype med_of_meds = kSmallest(medians, 0, numGroups - 1, (numGroups / 2) + 1, false, -1);

            //cout << "MEDIAN OF MEDIANS: " << med_of_meds << endl;

            return kSmallest(data, 0, size - 1, k, true, med_of_meds);
        }

        void stableSort()
        {
            reorder();
            //cout << "PRE SORT: ";
            printArray(data, size);
            mergeSort(data, 0, size - 1);
            //cout << "POST SORT: ";
            printArray(data, size);
        }

        //NOTE: Check with Dixon on how to convert to different values, as well as radix sorting a string
        //MAYBE convert all bitsets into bool arrays and do your own masking and management functions. This would and should be avoidable though.
        void radixSort(int i)
        {
            reorder();
            elmtype *data_sorted = new elmtype[capacity_arr];

            string bitMapStart = "";
            for(int x = 0; x < sizeof(elmtype) * 8; x++)
            {
                if(x < (sizeof(elmtype) * 8) - i)
                {
                    bitMapStart.append("0");
                }
                else
                {
                    bitMapStart.append("1");
                }
                
            }

            bitset<sizeof(elmtype) * 8> mask(bitMapStart);

            //cout << bitMapStart << endl;

            int numBins = pow(2, i);

            int bins[numBins];
            for(int k = 0; k < numBins; k++)
            {
                bins[k] = 0;
            }

            for(int j = 0; j < size; j++)
            {
                bitset<sizeof(elmtype) * 8> binaryVal(data[j]);
                bitset<sizeof(elmtype) * 8> maskedVal = mask & binaryVal;

                int maskedInt = (int) maskedVal.to_ulong();
                bins[maskedInt]++;

                //cout << maskedInt << endl;
            }

            //cout << "BINS ARR: ";
            //printArray(bins, numBins);
            int matchIdx = 0;

            for(int y = 0; y < numBins; y++)
            {
                //cout << "LOOP ITERATION #" << y+1 << endl;
                bitset<sizeof(elmtype) * 8> compareMask(y);

                for(int z = 0; z < size; z++)
                {
                    bitset<sizeof(elmtype) * 8> binaryVal(data[z]);

                    int testVal = (int)((mask & binaryVal).to_ulong());
                    //cout << "TEST VALUE: " << testVal << endl;

                    if((int)((mask & binaryVal).to_ulong()) == y)
                    {
                        //cout << "MATCH, ";
                        data_sorted[matchIdx] = data[z];
                        //cout << "DATA SORTED: " << data_sorted[matchIdx] << endl;
                        matchIdx++;
                    }
                }

            }

            cout << "RADIX SORTED ARR: ";
            printArray(data_sorted, size);

        }
        
        int linearSearch(elmtype e)
        {
            reorder();

            for(int i = 0; i < size; i++)
            {
                //cout << "DATA: " << data[i] << " vs SEARCHED: " << e << endl;
                if(data[i] == e)
                {
                    return i;
                }
            }

            return -1;


        }

        int binSearch(elmtype e)
        {
            return binarySearch(data, 0, size - 1, e);
        }

    private:
        int size;
        int capacity_arr;

        int head;
        int tail;

        elmtype error = -2147483627;

        elmtype* data;

        //NOTE: Call this prior to adding a value
        int resize(bool grow)
        {
            if(grow)
            {
                elmtype* data_resized = new elmtype[capacity_arr * 2];

                for(int i = 0; i < size; i++)
                {
                    data_resized[i] = data[(head + i) % capacity_arr];
                }

                capacity_arr *= 2;

                delete(data);
                data = data_resized;
                head = 0;
                tail = size - 1;

                return size;
            }
            else
            {
               elmtype* data_resized = new elmtype[capacity_arr / 2];

                for(int i = 0; i < size; i++)
                {
                    data_resized[i] = data[(head + i) % capacity_arr];
                }

                capacity_arr /= 2;

                delete(data);
                data = data_resized;
                head = 0;
                tail = size - 1;

                return size; 
            }
            
        }

        void reorder()
        {
            elmtype* reordered = new elmtype[capacity_arr];

                for(int i = 0; i < size; i++)
                {
                    reordered[i] = data[(head + i) % capacity_arr];
                }

            delete(data);
            data = reordered;
            head = 0;
            tail = size - 1;
        }

        int partition(elmtype arr[], int left, int right, int part_val)
        {
            elmtype partition_val = arr[part_val];
            int i = 0;

            /*
            for(int z = 0; z < size; z++)
            {
                cout << arr[z] << endl;
            }

            cout << "------------" << endl;
            */

            for(int j = left; j <= right - 1; j++)
            {
                if(arr[j] <= partition_val)
                {
                    //cout << arr[j] << endl;
                    swap(arr[i], arr[j]);
                    i++;
                }
            }
            swap(arr[i], arr[right]);

            /*
            cout << "------------" << endl;
            for(int z = 0; z < size; z++)
            {
                cout << arr[z] << endl;
            }
            cout << "I VAL: " << i << endl;
            //raise(SIGSEGV);
            */

            return i;
        }

        int kSmallest(elmtype arr[], int left, int right, int k, bool opt_part, int median)
        {
            //cout << k << " vs " << right << endl;
            if(k > 0 && k <= right - left + 1)
            {
                int part_idx;

                if(opt_part)
                {
                    part_idx = partition(arr, left, right, median);
                }
                else
                {
                    part_idx = partition(arr, left, right, right);
                }

                //cout << k << " vs " << part_idx << endl;

                if(part_idx == k - 1)
                {
                    return arr[part_idx];
                }

                if(k - 1 < part_idx)
                {
                    //cout << "Move Left" << endl;
                    return kSmallest(arr, left, part_idx - 1, k, opt_part, median);
                }

                if(k - 1 > part_idx)
                {
                    //cout << "Move Right" << endl;
                    //raise(SIGSEGV);
                    return kSmallest(arr, part_idx + 1, right, k - part_idx + left - 1, opt_part, median);  
                }


            }

            return 2147483627; //Integer maximum
        }

        void merge(elmtype *arr, int left, int middle, int right) 
        {
            elmtype temp[right - left + 1];
        
            int i = left, j = middle + 1, k = 0;
        
            while(i <= middle && j <= right)
            {
                if(arr[i] <= arr[j])
                {
                    temp[k] = arr[i];
                    k += 1; i += 1;
                }
                else
                {
                    temp[k] = arr[j];
                    k += 1; j += 1;
                }
            }

            while(i <= middle)
            {
                temp[k] = arr[i];
                k += 1; i += 1;
            }
        
            while(j <= right)
            {
                temp[k] = arr[j];
                k += 1; j += 1;
            }
        
            for(i = left; i <= right; i += 1)
            {
                arr[i] = temp[i - left];
            }
        }
        
        void mergeSort(elmtype *arr, int left, int right)
        {
            if(left < right)
            {
                int middle = (left + right) / 2;

                mergeSort(arr, left, middle);
                mergeSort(arr, middle + 1, right);

                merge(arr, left, middle, right);
            }
        }

        void printArray(elmtype A[], int size) 
        { 
            for (int i=0; i < size; i++)
            {
                cout << A[i];
            }

            printf(" \n"); 
        } 

        int binarySearch(elmtype arr[], int left, int right, int toSearch) 
        { 
            if (right >= left) 
            { 
                int middle = left + (right - left) / 2; 
          
                if (arr[middle] == toSearch) 
                    return middle; 
          
                // If element is smaller than mid, then 
                // it can only be present in left subarray 
                if (arr[middle] > toSearch) 
                    return binarySearch(arr, left, middle - 1, toSearch); 
          
                // Else the element can only be present 
                // in right subarray 
                return binarySearch(arr, middle + 1, right, toSearch); 
            } 
          
            // We reach here when element is not 
            // present in array 
            return -1; 
        } 
};

