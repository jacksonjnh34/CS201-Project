#include <iostream>

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
            //free(data);
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
        int clear()
        {
            this = new CircularDynamicArray();
        }

        elmtype QuickSelect(int k);
        elmtype WCSelect(int k);

        void stableSort();
        void radixSort(int i);
        
        int linearSearch(elmtype e);
        int binSearch(elmtype e);

    private:
        int size;
        int capacity_arr;

        int head;
        int tail;

        int error = -2147483627;

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
};

