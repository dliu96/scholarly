/* Interface for a key-value pair */
public interface Entry<Key,Value> {
	Key getKey();
	Value getValue();	
}