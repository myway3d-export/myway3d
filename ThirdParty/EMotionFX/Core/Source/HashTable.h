/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __HASHTABLE_H
#define __HASHTABLE_H

// include the needed headers
#include "Macros.h"
#include "Config.h"
#include "SourceDeclaration.h"
#include "Array.h"
#include "FastMath.h"
#include <assert.h>
#include "HashFunctions.h"


namespace MCore
{

/**
 * The Hash Table template.
 * Hash tables can be used to speedup searching of specific values based on a key.
 * The table has an array of table elements, which each can contain multiple hash table entries.
 * Each entry is identified by a unique key, which can be of any type, as long as the == operator is specified.
 * Next to a unique key, every entry contains a value. The hash table implementation contains methods to add new
 * entries and to retrieve the value for a given key. Hashing is used to speedup this search.
 * The hash value of a given key is calculated based on a specified hashing function that you pass to the constructor.
 * This hash function will return a non-negative (so positive) integer based on the input key.
 * Performance tests have shown that you need at least 100 entries to make it faster than linear searches. This however
 * also depends on the speed of your hash function and some other factors. But it is a good practise to replace your
 * linear searches by a hash table when you have more than 100 items to search through. The more items to search through
 * the bigger the advantage of hashing over linear searches will be. Here follows a small table that shows how much faster your 
 * searches can be compared to linear searches:
 *
 * <pre>
 * --------------------------
 * Num Items | Speedup Factor
 * --------------------------
 *        10 | ~ 0.3x
 *       100 | ~ 1x    (so after more than 100 items you start to gain real speed)
 *      1000 | ~ 8x
 *     10000 | ~ 60x
 *    100000 | ~ 120x
 * --------------------------
 * </pre>
 *
 * Those tests assume a that the hash table contains 1000 elements. Lower element counts will produce lower speedup
 * gains with bigger number of items. The tests perform the same number of searches as there are items/entries.
 */
template <class Key, class Value>
class HashTable
{
	public:
		/**
		 * A hash table entry, which contains a unique key and a value for this key.
		 */
		class Entry
		{
			friend class MCore::HashTable;

			public:
				/**
				 * The constructor.
				 * @param key The unique key of this entry.
				 * @param value The value linked to this key.
				 */
				inline Entry(const Key& key, const Value& value) : mKey(key), mValue(value) {}

				/**
				 * Set the value of this entry.
				 * @param value The value to set for this entry.
				 */
				inline void SetValue(const Value& value)	{ mValue = value; }

				/**
				 * Get the value of this entry.
				 * @result The value that is linked to the key of this entry.
				 */
				inline const Value& GetValue() const		{ return mValue; }

				/**
				 * Get the key of this entry.
				 * @result The unique key of this entry.
				 */
				inline const Key& GetKey() const			{ return mKey; }

			private:
				Key		mKey;		/**< The unique key. */
				Value	mValue;		/**< The value that is linked to the given key. */
		};


		/**
		 * The default constructor.
		 * This creates an empty hash table. You need to call the Init function before you can use the table.
		 * @see Init
		 */
		HashTable();

		/**
		 * The extended constructor, which also initializes the table automatically.
		 * You do NOT need to call the Init function anymore when you use this constructor.
		 * @param maxElements The maximum number of table elements. The higher the value, the more gain when dealing with many entries.
		 *                    Values between 100 and 1000 are often good numbers, depending on the number of entries you are dealing with.
		 * @param hashFunction The hash function object to use. This calculates the hash value for a given key.
		 */
		HashTable(const int maxElements, HashFunction<Key>* hashFunction);

		/**
		 * Copy constructor.
		 * @param other The table to create a copy of.
		 */
		HashTable(const HashTable<Key, Value>& other);

		/**
		 * The destructor.
		 * This automatically clears all table entries.
		 * The hash function object that was passed to the extended constructor or the Init function will be
		 * deleted from memory automatically.
		 */
		~HashTable();

		/**
		 * Clear the hash table.
		 * This removes all entries from the table. If you like to use the table again later on you will need to 
		 * call the Init function again.
		 * This also automatically deletes the hash function object, that you passed to the extended constructor or init function, from memory.
		 * @see Init
		 */
		void Clear();

		/**
		 * Locate an entry with a given key.
		 * When the entry cannot be found, this method will NOT modify the outElementNr and outEntryNr parameters.
		 * @param key The key to search for.
		 * @param outElementNr A pointer to an integer in which this method will store the table element number, in case the entry is found.
		 * @param outEntryNr A pointer to an integer in which this method will store the entry number (index) into the table element array, in case the entry is found.
		 * @result Returns true when the entry with the given key could be found, otherwise false is returned.
		 */
		inline bool FindEntry(const Key& key, int* outElementNr, int* outEntryNr) const;

		/**
		 * Initialize the hash table.
		 * @param maxElements The maximum number of table elements. The higher the value, the more gain when dealing with many entries.
		 *                    Values between 100 and 1000 are often good numbers, depending on the number of entries you are dealing with.
		 * @param hashFunction The hash function object to use. This calculates the hash value for a given key.
		 */
		void Init(const int maxElements, HashFunction<Key>* hashFunction);

		/**
		 * Add an entry to the hash table.
		 * It is VERY important that the key is unique and does NOT already exist within this table!
		 * @param key The unique key of the entry.
		 * @param value The value that is linked to this key.
		 */
		void Add(const Key& key, const Value& value);

		/**
		 * Get a value from the table.
		 * @param inKey The key of the entry which contains the value.
		 * @param outValue A pointer to an object where this method will write the value of the entry in.
		 * @result Returns true when the value has been retrieved successfully. False will be returned when no entry with the specified
		 *         key could be located.
		 */
		inline bool GetValue(const Key& inKey, Value* outValue) const;

		/**
		 * Set the value that is linked to a given key.
		 * @param key The unique key of the entry to set the value for.
		 * @param value The value to link to the specified key.
		 * @result Returns true when the value has been set successfully, or false when there is no entry with the specified key inside this table.
		 */
		inline bool SetValue(const Key& key, const Value& value);

		/**
		 * Check if this hash table contains an entry with a specified key.
		 * @param key The key of the entry to search for.
		 * @result Returns true when this hash table contains an entry with the specified key, otherwise false is returned.
		 */
		inline bool Contains(const Key& key) const;

		/**
		 * Remove the entry which has the specified key.
		 * @param key The key of the entry to remove.
		 * @result Returns true when the entry with the specified key has been removed successfully, otherwise false is returned, which means
		 *         that there is no entry with the specified key.
		 */
		bool Remove(const Key& key);

		/**
		 * Get the number of table elements.
		 * @result The number of table elements.
		 */
		inline int GetNumTableElements() const;

		/**
		 * Get the number of entries in a given table element.
		 * @param tableElementNr The table element number to get the number of entries for.
		 * @result The number of entries for the specified table entry.
		 */
		inline int GetNumEntries(const int tableElementNr) const;

		/**
		 * Get the total number of entries inside the table.
		 * @result The total number of stored entries inside the table.
		 */
		int GetTotalNumEntries() const;

		/**
		 * Calculate the load balance, which is a percentage that represents how many percent of the
		 * table elements are used. If the returned value equals 50, then it means that 50 percent of the
		 * table elements are storing entries. The other 50% are then not used.
		 * When you have added many entries (more than then the number of table elements), and the load balance
		 * is not 100% or anywhere near it, it means your hash function is very inefficient, because it does not spread
		 * the entries over the entire hash table, which might mean that there is some nasty clustering going on, which can
		 * greatly decrease performance.
		 * @result A floating point value in range of 0..100, which respresents the percentage of table elements that is in use.
		 */
		float CalcLoadBalance() const;

		/**
		 * Calculate the average number of entries per used table element.
		 * The more entries per table element, the slower your searches will be.
		 * The optimal value returned by this function would therefore be 1.
		 * @result The average number of entries per table element.
		 */
		float CalcAverageNumEntries() const;

		/**
		 * Get a given entry from the table, when you know its location in the table.
		 * @param tableElementNr The table element number.
		 * @param entryNr The entry number inside this table element.
		 * @result The reference to the entry, with write access.
		 */
		inline Entry& GetEntry(const int tableElementNr, const int entryNr);

		/**
		 * The assignment operator.
		 * This clones the entire table.
		 * @param other The table to create a copy of.
		 * @result The copied version of the specified table.
		 */
		HashTable<Key, Value>& operator = (const HashTable<Key, Value>& other);

	protected:
		MCore::Array< MCore::Array<Entry>* >	mElements;			/**< The table elements, where NULL means the element is empty. */
		HashFunction< Key >*					mHashFunction;		/**< The hash function to use for this table. The hash function is deleted from memory automatically on destruction of the table. */
		int										mTotalNumEntries;	/**< The cached number of entries in the table. */
};


// include the inline code
#include "HashTable.inl"


}	// namespace MCore

#endif