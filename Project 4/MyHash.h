// MyHash.h

// Skeleton for the MyHash class template.  You must implement the first seven
// member functions; we have implemented the eighth.

#ifndef MYHASH_INCLUDED
#define MYHASH_INCLUDED

const int DEFAULTNUMBUCKETS = 100;

template<typename KeyType, typename ValueType>
class MyHash
{
public:
    MyHash(double maxLoadFactor = 0.5);
    ~MyHash();
    void reset();
    void associate(const KeyType& key, const ValueType& value);
    int getNumItems() const;
    double getLoadFactor() const;
    
    // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const;
    
    // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const MyHash*>(this)->find(key));
    }
    
    // C++11 syntax for preventing copying and assignment
    MyHash(const MyHash&) = delete;
    MyHash& operator=(const MyHash&) = delete;
    
private:
    
    struct Bucket
    {
        KeyType m_key;
        ValueType m_value;
        Bucket* m_next;
    };
    
    //hash table (dynamic array)
    Bucket** m_hashTable;
    
    //max load factor
    double m_maxLoadFactor;
    int m_numBuckets;
    int m_numItems;
    
    //private helper functions
    void initializeHashTable(Bucket**& hashTable, int numBuckets);
    void deleteHashTable(Bucket**& hashTable, int numBuckets);
    unsigned int getBucketNumber(const KeyType& key, int numBuckets) const;
    
    
    //REMEMEBER INSERT DOES NOT HANDLE INCREMENTING M_NUMITEMS
    void insertNewBucket(const KeyType& key, const ValueType& value, Bucket** hashTable, int numBuckets);
    
};

//IMPLEMENTATIONS

template<typename KeyType, typename ValueType>
MyHash<KeyType, ValueType>::MyHash(double maxLoadFactor)
{
    initializeHashTable(m_hashTable, DEFAULTNUMBUCKETS);
    
    if (maxLoadFactor <= 0)
    {
        m_maxLoadFactor = 0.5;
    }
    else if (maxLoadFactor > 2.0)
    {
        m_maxLoadFactor = 2.0;
    }
    else
    {
        m_maxLoadFactor = maxLoadFactor;
    }
    
    m_numBuckets = DEFAULTNUMBUCKETS;
    m_numItems = 0;
}

template<typename KeyType, typename ValueType>
MyHash<KeyType, ValueType>::~MyHash()
{
    deleteHashTable(m_hashTable, m_numBuckets);
}

template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::reset()
{
    deleteHashTable(m_hashTable, m_numBuckets);
    m_numBuckets = DEFAULTNUMBUCKETS;
    m_numItems = 0;
    
    initializeHashTable(m_hashTable, m_numBuckets);
}

template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
    ValueType* currentValue = find(key);
    if (currentValue != nullptr)   //the key is already in the hash table
    {
        *currentValue = value;
    }
    else    //the key isn't already in the hash table, so add a new bucket
    {
        insertNewBucket(key, value, m_hashTable, m_numBuckets);
        
        m_numItems++;
    }
    
    
    //check for load factor too big
    if (getLoadFactor() > m_maxLoadFactor)
    {
        //declare and initialize a temporary hash table
        Bucket** tempHashTable;
        initializeHashTable(tempHashTable, m_numBuckets * 2);
        

        //iterate through the current hash table
        for (int i = 0; i < m_numBuckets; i++)
        {
            Bucket* currentBucket = m_hashTable[i];
            
            
            while (currentBucket != nullptr)   //while there are items at the current bucket
            {
                KeyType currentKey = currentBucket->m_key;
                ValueType currentValue = currentBucket->m_value;
                
                //insert the bucket into the new hash table at the appropriate bucket
                insertNewBucket(currentKey, currentValue, tempHashTable, m_numBuckets * 2);
                
                //increment current bucket
                currentBucket = currentBucket->m_next;
            }
        }
        
        deleteHashTable(m_hashTable, m_numBuckets);
        m_numBuckets *= 2;

        initializeHashTable(m_hashTable, m_numBuckets);
     
        for (int i = 0; i < m_numBuckets; i++)
        {
            m_hashTable[i] = tempHashTable[i];
        }
        
        delete [] tempHashTable;
    }
}

template<typename KeyType, typename ValueType>
int MyHash<KeyType, ValueType>::getNumItems() const
{
    return m_numItems;
}

template<typename KeyType, typename ValueType>
double MyHash<KeyType, ValueType>::getLoadFactor() const
{
    return static_cast<double>(m_numItems) / m_numBuckets;
}

template<typename KeyType, typename ValueType>
const ValueType* MyHash<KeyType, ValueType>::find(const KeyType& key) const
{
    //get the bucket that the key would map to
    unsigned int bucketNumber = getBucketNumber(key, m_numBuckets);
    
    //iterate through that bucket's linked list to find the key
    Bucket* head = m_hashTable[bucketNumber];  //check
    Bucket* p = head;
    
    while (p != nullptr)
    {
        if (p->m_key == key)    //we found the key
        {
            return &p->m_value;     //return a pointer to the value associated with the key
        }
        
        p = p->m_next;
    }
    
    return nullptr; //return nullptr if the key cannot be found
}

template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::initializeHashTable(Bucket**& hashTable, int numBuckets)
{
    hashTable = new Bucket*[numBuckets];
    
    //set everything to nullptr
    for (int i = 0; i < numBuckets; i++)
    {
        hashTable[i] = nullptr;
    }
}

template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::deleteHashTable(Bucket**& hashTable, int numBuckets)
{
    //delete all the nodes in each linked list
    for (int i = 0; i < numBuckets; i++)
    {
        Bucket* p = hashTable[i];
        
        while (p != nullptr)
        {
            Bucket* n = p->m_next;
            delete p;
            p = n;
        }
    }
    
    //delete the table itself
    delete [] hashTable;
}

template<typename KeyType, typename ValueType>
unsigned int MyHash<KeyType, ValueType>::getBucketNumber(const KeyType& key, int numBuckets) const
{
    unsigned int hash(const KeyType& k);    //prototype
    
    unsigned int hashValue = hash(key);
    
    unsigned int bucketNum = hashValue % numBuckets;
    return bucketNum;
    
}

template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::insertNewBucket(const KeyType& key, const ValueType& value, Bucket** hashTable, int numBuckets)
{
    //make a new Bucket
    Bucket* newBucket = new Bucket;
    newBucket->m_key = key;
    newBucket->m_value = value;
    newBucket->m_next = nullptr;
    
    unsigned int bucketNumber = getBucketNumber(key, numBuckets);
    
    Bucket* head = hashTable[bucketNumber];  //check this
    Bucket* p = head;
    
    if (p == nullptr)   //its empty, so add to front
    {
        hashTable[bucketNumber] = newBucket;
    }
    else   //add to back otherwise (the linked list is not empty)
    {
        //the loop continues until p points at the last node in the list
        while (p->m_next != nullptr)
        {
            p = p->m_next;
        }
        
        //p (now the 2nd to last item) should now have next point to the new node
        p->m_next = newBucket;
    }
}

#endif // MYHASH_INCLUDED
