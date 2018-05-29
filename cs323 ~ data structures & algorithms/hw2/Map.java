import java.util.Collection;

public interface Map<K,V> 
{
  /** Returns the number of items in the map. */ 
  public int size();

  /** Returns whether the map is empty. */
  public boolean isEmpty();

  /** Puts a given key-value pair in the map, replacing a previous
    * one, if any, and returns the old value. */
  public V put(K key, V value);

  /** Returns the value associated with a key. */
  public V get(K key);

  /** Removes the key-value pair with a given key. */
  public V remove(K key);
}