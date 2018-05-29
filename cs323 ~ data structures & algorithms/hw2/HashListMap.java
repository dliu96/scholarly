/*
THIS CODE WAS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
CODE WRITTEN BY OTHER STUDENTS OR SOURCES OUTSIDE OF THOSE
PROVIDED BY THE INSTRUCTOR. Derek Liu
*/

public class HashListMap<K, V> implements Map<K, V> {
  public HashListEntry<K,V>[] bucket;   // The hash table (buckets)
  public int capacity;                  // capacity == bucket.length
  public int NItems;                    // NItems = # entries in hash table

  public int MAD_p;
  public int MAD_a;
  public int MAD_b;

  /* ===============================================================
  General Constructor:  initialize prime number p
    Create an array (bucket) of given size
  =============================================================== */
  public HashListMap(int p, int MapArraySize) {
    capacity = MapArraySize;
    bucket = new HashListEntry[ capacity ]; // Java can't create gen arrays
    NItems = 0;

    java.util.Random rand = new java.util.Random(1234);

    MAD_p = p;                              // Prime number in MAD
    MAD_a = rand.nextInt(MAD_p-1) + 1;      // Multiplier in MAD
    MAD_b = rand.nextInt(MAD_p);            // Shift amount in MAD
  }

  /* ===============================================================
  Common Constructor:  pick the default prime number 1683553
   Create an array (bucket) of given size
  =============================================================== */
  public HashListMap(int MapArraySize) {
    this( 1683553, MapArraySize );   // Calls general constructor
  }

  public HashListMap() {
    this( 1683553, 1000 );   // Default array size = 1000
  }


  public int size() {
    return NItems;
  }

  public boolean isEmpty() {
    return NItems==0;
  }

  /* ===============================================================
  Hash function:

  1. uses the Hash Code inside the K class
  2. uses the MAD compression function

  hash index = [ (a*HashCode + b) % p ] % N
  =============================================================== */
  public int hashValue(K key) {
    int x = key.hashCode();      // K has a built-in hashCode() !!!

    return (int) ((Math.abs(x*MAD_a + MAD_b) % MAD_p) % capacity);
  }

  // ****************************************************************
  // Todo: implement these 3 methods
  //
  //     1. insert()
  //     2. delete()
  //     3. findEntry()
  //
  //  See project description for details
  // ****************************************************************


  // ****************************************************************
  // insert(bucketID, e): insert entry e in bucket[bucketID]
  //
  // Write this method
  // ****************************************************************
  public void insert(int bucketID, HashListEntry<K, V> e) {
	  
	  //check to see if we should resize our map
	  boolean resize = (NItems + 1) > (capacity/2);
	  //System.out.println("NItems = " + NItems);
	  
			  
	  if(bucket[bucketID] == null){
		  //there is nothing else here
		  bucket[bucketID] = e;
		  
		  //System.out.println("Inserting ( "+ e.getKey() + "," + e.getValue() + " ) at bucketID = " + bucketID);	
		  
		  //increase size
		  NItems++; 
	  }
	  else{
		  //there is already another entry with this hash!
		  
		  //variables to traverse the links
		  HashListEntry<K, V> temp = bucket[bucketID];
		  HashListEntry<K, V> prevElem;
		  
		  //looking for the end
		  while(temp.getKey() != e.getKey()){
			  
			  if(temp.next != null){
				  
				  // our next entry has the same key as the one we're tryin to insert
				  if(temp.next.getKey() == e.getKey()){
					  // override old entry
					  temp.next = e;
					  //System.out.println("Inserting ( "+ e.getKey() + "," + e.getValue() + " ) at bucketID = " + bucketID);
					  
					  //our job here is done
					  break;
				  }
				  
				  //keep traversing
				  temp = temp.next;
				  continue;
			  }
			  
			  // insert new linked entry at its proper location
			  temp.next = e;
			  //System.out.println("Inserting ( "+ e.getKey() + "," + e.getValue() + " ) at bucketID = " + bucketID);			  
			  // update the prev pointer
			  prevElem = temp;
			  temp = temp.next;
			  temp.prev = prevElem;
			  
			  //adjust size for the new entry
			  NItems++;
			  
			  //our job here is done
			  break;
			  
		  }
		  
	  }
	  
	  if(resize){
		  System.out.println("Resize = " + resize);
		  resize();
	  }
  }
  
  public void resize()
  {
	  	//Our old entries
	  	HashListEntry<K,V>[] old = bucket;
	  	
	  	//the next prime after 2M
	  	int newCap = segmentedSieve(2*capacity, 8*capacity);
	  	//System.out.println("new capacity = " + newCap);
		
	  	//reset map for replacement
		capacity = newCap;
		bucket = new HashListEntry[ capacity ];
		NItems = 0;
		
//		java.util.Random rand = new java.util.Random(1234);
//
//	    MAD_a = rand.nextInt(MAD_p-1) + 1;      // Multiplier in MAD
//	    MAD_b = rand.nextInt(MAD_p);            // Shift amount in MAD
//	    
	    //put all the old values back in our newly grown map
	    for(int i = 0 ; i<old.length; i++)
	    {
	    	//not all indices/hashes will have an entry
	    	if(old[i] != null){
		    	HashListEntry<K,V> e;
		    	if(old[i].next == null){
		    		//single entry
		    		e = old[i];
		    		
		    		//insert entry
		    		put(e.getKey(),e.getValue());
		    	}
		    	else
		    	{
		    		//our current position has links
		    		HashListEntry<K, V> temp = old[i];
		    		while(temp != null){
		    			//insert entry at the end of the existing links
		    			put(temp.getKey(),temp.getValue());
		    			temp = temp.next;
		    		}
		    	}
	    	}
	    }
	  
  }
  
  //method to calculate prime numbers between a lower and upper bound
  public int segmentedSieve(int lower, int upper) {
	  //how we will keep track of our primes
	  int array[];
	  int primes[];

	  int j = 0;
	  int sqt = (int) Math.sqrt(upper);

	  //initialize our arrays
	  array = new int[sqt + 1];
	  primes = new int[sqt + 1];
	  //System.out.println("sqt:" + sqt);

	  //initialise(sqt + 1);
	  for (int i = 0; i < sqt + 1; i++) {
		  //set every number to 1 to indicate the possibility of prime
		  array[i] = 1;
	  }

	  //the sieve up to the sqt(upper)
	  for (int i = 2; i <= sqt; i++) {
		  //we have a prime
		  if (array[i] == 1) {
			  primes[j] = i;
			  j++;

			  //mark multiples of our prime off
			  for (int k = i + i; k <= sqt; k += i) {
				  array[k] = 0;
			  }
		  }
	  }

	  //get an idea for how many primes we're looking for
	  int diff = (upper - lower + 1);
	  array = new int[diff];

	  //initialise(diff);
	  for (int i = 0; i < diff; i++) {
		  //set every number to 1 to indicate the possibility of prime
		  array[i] = 1;
	  }

	  //the segmented sieve algorithm
	  int k = 0;
	  while (k < j) {
		  int div = lower / primes[k];
		  div *= primes[k];
		  while (div <= upper) {
			  if(div>=lower && primes[k]!=div)
				  array[div-lower] = 0;
			  div += primes[k];
		  }

		  k++;
	  }

	  //variable to keep track of the first prime
	  int nextPrime = 0;

	  for (int i = 0; i < diff; i++) {
		//number is prime
	  	if (array[i] == 1 && (i+lower) !=1){
	  		//System.out.println(i + lower);
	  		nextPrime = i + lower;
	  		//we are only looking for the first prime
	  		break;
	  	}
	  }
	  return nextPrime;
  	
  }
  


  // *****************************************************************
  // delete(bucketID, e): delete entry e from bucket[bucketID]
  //
  // Write this method
  // *****************************************************************
  public void delete(int bucketID, HashListEntry<K, V> p) {
	    
	  HashListEntry<K, V> temp = bucket[bucketID];
	  
	  // last element
	  if(temp.next == null){
		  bucket[bucketID] = null;
		  NItems--;
	  }
	  else{
		  //iterate through links if present
		  while(temp != null)
		  {
			  
			  // find the matching entry
			  if(temp.getKey() == p.getKey()){
				  
				  HashListEntry<K, V> prevElem, nextElem;
				  
				  //keep track of prev and next pointers
				  prevElem = temp.prev;
				  nextElem = temp.next;

				  
				  if(prevElem == null && nextElem != null){ //first element in linked list
					  //remove pointers to this entry
					  nextElem.prev = null;
					  
					  //set the second entry as the head reference
					  bucket[bucketID] = nextElem;
					  
					  //decrease map variable
					  NItems--;
				  }
				  else if(prevElem != null && nextElem == null){ //last element in linked list
					  //remove pointers to this entry
					  prevElem.next = null;
					  
					  //remove this entry
					  temp = null;
					  
					  //decrease map variable
					  NItems--;
				  }
				  else{ //middle element in linked list
					  
					  //edit pointers to and from this entry
					  prevElem.next = nextElem;
					  nextElem.prev = prevElem;
					  
					  //remove entry
					  temp = null;
					  
					  //decrease map variable
					  NItems--;
				  }
				  //we're done after the key has been found and removed
				  break;
				  
			  }
			  
			  //iterate trhough links
			  temp = temp.next;
		  }
	  
	  }
	  

  }
  


  // *********************************************************************
  // findEntry(key): find entry for given key
  //
  // Return value:
  //
  //       The reference to entry that contains the given key
  //       If key is NOT found, return null
  //
  // Write this method
  // ********************************************************************* 
  public HashListEntry<K,V> findEntry(K key) {
	  int bucketID;
	  
	  if ( key != null ){ //just in case...
		  
		  // Start looking for key
		  bucketID = hashValue(key);
		  
		  //in case we need to traverse any links
		  HashListEntry<K,V> temp = bucket[bucketID];
		  
		  if(temp != null){
			  while(temp.getKey() != key){
				  if(temp.next == null){ 
					  //System.out.println("Key not found.");
					  return null;
				  }
				  //traverse links
				  temp = temp.next;
			  }
			  
			  if(temp.getKey() == key){
				  //we found what we came for
				  return temp;
			  }
			  else{
				  //System.out.println("Key not found.");
				  return null;
			  }
		  }

	  }
	  //System.out.println("Key cannot be null. ");
	  return null;

  }



  /* ===========================================================
  get(), put() and remove() are very easily implemented
  using 

  findEntry()
  insert()
  delete()
  =========================================================== */

  public V get(K k){
    HashListEntry<K, V> e;

    e = findEntry(k);  // Find entry for the key k

    if ( e != null ) {
      return e.getValue();       // return value
    }
    else {
      return null;               // Key not found
    }
  }


  public V put (K k, V v) {
    HashListEntry<K, V> e;

    e = findEntry(k);  // Find entry for the key k

    if ( e != null ) {
      V oldValue = e.setValue(v);// set new value
      return ( oldValue );  // Return old value
    }
    else {
      int bucketID = hashValue( k );
      
      //System.out.println("PUT Inserting key ( "+ k + " ) at bucketID = " + bucketID);

      insert( bucketID, new HashListEntry<K,V>(k, v) );

      return null;
    }
  }


  public V remove(K k) {
    HashListEntry<K, V> e = findEntry(k);  // Find entry for the key k

    if ( e != null ) {
      V oldValue = e.getValue();

      int bucketID = hashValue( k );
      delete ( bucketID, e );

      return oldValue;
    }
    else {
      return null;               // Key not found
    }
  }

  public String toString() {
    boolean more;

    String output = "{";

    for (int i = 0; i < bucket.length; i++) {
      HashListEntry<K,V> e;

      e = bucket[i];

      if ( e != null && e.next != null ) {
        more = true;
        output += "[";
      }
      else
        more = false;

      while ((e != null) ) { 
        output = output + e + " ";
        e = e.next;
      }

      if ( more )
        output += "] ";
    }

    output += "} (" + NItems + " entries)";
    return output;
  }
}