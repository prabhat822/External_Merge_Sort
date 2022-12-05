
#include <iostream>
#include <climits>
#include <vector>
#include <time.h>
using namespace std;

struct ListNode
{
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
    int val;
};

vector<ListNode *> newlists1;
void adjust(int a[], int winner, vector<ListNode *> lists)
{
    int curP = winner;
    int k = lists.size() - 1;
    for (int i = (k + winner) / 2; i > 0; i = i / 2)
    {
        // parent is winner, set curP to the loser node(a[i]), and set parent node(old a[i] value) to curP
        if (lists[a[i]]->val <= lists[curP]->val)
        {
            int tmp = a[i];
            a[i] = curP;
            curP = tmp;
        }
    }
    a[0] = curP;
}

ListNode *mergeKLists(vector<ListNode *> &lists)
{
    vector<ListNode *> newlists;
    int t = 0, p = 0;

    if (lists.empty())
        return NULL;

    for (int i = 0; i < lists.size(); i++)
    {
        //     cout<<lists[i];
        if (!lists[i])
        {
            t++;
        }
        else
        {
            newlists.push_back(lists[i]);
        }
    }

    if (t == lists.size())
        return NULL;

    if (newlists.size() == 0)
    {
        return NULL;
    }
    if (newlists.size() == 1)
    {
        return newlists[0];
    }
    ListNode *head = new ListNode(0);
    int k = newlists.size();
    // 0 - winner, 1~k-1:loser, k+1: MIN_VALUE
    int a[k];
    vector<ListNode *> b(k + 1);
    b[k] = new ListNode(INT_MIN);
    for (int i = 0; i < k; i++)
    {
        a[i] = k;
        b[i] = newlists[i];
    }

    // init loser tree
    for (int i = k - 1; i >= 0; i--)
    {
        adjust(a, i, b);
    }

    int finishedCount = 0;
    ListNode *cur = head;
    while (finishedCount < k)
    {
        int winner = a[0];
        cur->next = b[winner];
        cur = cur->next;
        if (cur->next != NULL)
        {
            b[winner] = cur->next;
        }
        else
        {
            b[winner] = new ListNode(INT_MAX);
            finishedCount++;
        }
        adjust(a, winner, b);
    }

    return head->next;
}

void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    /* create temp arrays */
    int L[n1], R[n2];

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    /* Merge the temp arrays back into arr[l..r]*/
    // Initial index of first subarray
    i = 0;

    // Initial index of second subarray
    j = 0;

    // Initial index of merged subarray
    k = l;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    /* Copy the remaining elements of L[],
        if there are any */
    while (i < n1)
        arr[k++] = L[i++];

    /* Copy the remaining elements of R[],
        if there are any */
    while (j < n2)
        arr[k++] = R[j++];
}

/* l is for left index and r is right index of the
   sub-array of arr to be sorted */
void mergeSort(int arr[], int l, int r)
{
    if (l < r)
    {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l + (r - l) / 2;

        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}

FILE *openFile(char *fileName, char *mode)
{
    FILE *fp = fopen(fileName, mode);
    if (fp == NULL)
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }
    return fp;
}
void insert(ListNode **root, int item)
{
    ListNode *temp = new ListNode;
    ListNode *ptr;
    temp->val = item;
    temp->next = NULL;

    if (*root == NULL)
        *root = temp;
    else
    {
        ptr = *root;
        while (ptr->next != NULL)
            ptr = ptr->next;
        ptr->next = temp;
    }
}
ListNode *arrayToList(int arr[], int i)
{
    ListNode *root = NULL;
    for (int j = 0; j < i; j++)
        insert(&root, arr[j]);
    return root;
}

void createInitialRuns(char *input_file, int run_size,
                       int num_ways)
{
    // For big input file
    FILE *in = openFile(input_file, "r");

    // allocate a dynamic array large enough
    // to accommodate runs of size run_size
    int *arr = (int *)malloc(run_size * sizeof(int));

    bool more_input = true;
    int next_output_file = 0;

    int i;
    while (more_input)
    {
        // write run_size elements
        // into arr from input file
        for (i = 0; i < run_size; i++)
        {
            if (fscanf(in, "%d ", &arr[i]) != 1)
            {
                more_input = false;
                break;
            }
        }

        // sort array using merge sort
        mergeSort(arr, 0, i - 1);

        // write the records to the
        // appropriate scratch output file
        // can't assume that the loop
        // runs to run_size
        // since the last run's length
        // may be less than run_size

        ListNode *root = arrayToList(arr, i);
        newlists1.push_back(root);
    }

    fclose(in);
}

void externalSort(char *input_file, char *output_file,
                  int num_ways, int run_size)
{
    FILE *out = openFile(output_file, "w");
    // read the input file,
    // create the initial runs,
    // and assign the runs to
    // the scratch output files
    createInitialRuns(input_file, run_size, num_ways);

    // Merge the runs using
    // the K-way merging
    // for(auto x : newlists1)
    //     cout<<x<<endl;
    // ListNode * head1 = newlists1[0];
    // while(head1!=NULL){
    //     cout<<head1->val<<" ";
    //     head1 = head1 -> next;
    // }

    ListNode *lnode = mergeKLists(newlists1);
    ListNode *head1 = lnode;
    //  while(head1!=NULL){
    //     cout<<head1->val<<" ";
    //     head1 = head1 -> next;
    // }

    while (head1 != NULL)
    {
        fprintf(out, "%d ", head1->val);
        head1 = head1->next;
    }
}

int main()
{
    // No. of Partitions of input file.
    int num_ways = 40000;

    // The size of each partition
    int run_size = 1000;

    char input_file[] = "input1.txt";
    char output_file[] = "output2.txt";

    // FILE* in = openFile(input_file, "w");

    // srand(time(NULL));

    // // generate input
    // for (int i = 0; i < num_ways * run_size; i++)
    //     fprintf(in, "%d ", rand());

    // fclose(in);

    externalSort(input_file, output_file, num_ways,
                 run_size);

    return 0;
}