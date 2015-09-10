import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

/**
 * Created by amaliujia on 15-9-10.
 */
public class SDConcurrencyHashMap<K, V> {
    private double load_factor = 0.75;
    private int default_slots = 16;
    private SDSlot<K, V>[] slots;
    private Lock[] locks;

    public SDConcurrencyHashMap(int s, double load_factor){
        this.load_factor = load_factor;
        this.default_slots = s;


        this.slots = new SDSlot[s];
        this.locks = new Lock[s];

        for(int i = 0; i < this.default_slots; i++){
            slots[i] = new SDSlot<K, V>();
            locks[i] = new ReentrantLock();
        }
    }

    public boolean containsKey(K key){
        int offset = key.hashCode() % this.slots.length;
        locks[offset].lock();
        SDSlot slot = this.slots[offset];
        boolean result = slot.containsKey(key);
        locks[offset].unlock();
        return result;
    }

    public void put(K key, V value){
        int offset = key.hashCode() % this.slots.length;
        locks[offset].lock();
        SDSlot slot = this.slots[offset];
        slot.addElement(key, value);
        locks[offset].unlock();
    }

    public V get(K key){
        int offset = key.hashCode() % this.slots.length;
        locks[offset].lock();
        SDSlot slot = this.slots[offset];
        V result = (V)slot.getElement(key);
        locks[offset].unlock();
        return result;
    }

    private class SDSlot<K, V>{
        K key;
        List<SDHashMapEntry<K, V>> entries = new LinkedList<SDHashMapEntry<K, V>>();


        public boolean containsKey(K key){
            Iterator<SDHashMapEntry<K, V>> iter = entries.iterator();
            while (iter.hasNext()){
                SDHashMapEntry<K, V> entry = iter.next();
                if(entry.key == key){

                    return true;
                }
            }
            return false;
        }


        public void addElement(K key, V value){
            Iterator<SDHashMapEntry<K, V>> iter = entries.iterator();
            while (iter.hasNext()){
                SDHashMapEntry<K, V> entry = iter.next();
                if(entry.key == key){
                    entry.value = value;
                    return;
                }
            }

            SDHashMapEntry<K, V> entry = new SDHashMapEntry<K, V>();
            entry.key = key;
            entry.value = value;
            entries.add(entry);
        }

        public V getElement(K key){
            Iterator<SDHashMapEntry<K, V>> iter = entries.iterator();
            while (iter.hasNext()){
                SDHashMapEntry<K, V> entry = iter.next();
                if(entry.key == key){
                    V result = entry.value;
                    iter.remove();
                    return result;
                }
            }
            return null;
        }
    }

    private class SDHashMapEntry<K, V>{
        public K key;
        public V value;
    }


    public static void main(String[] args){
        SDConcurrencyHashMap<String, String> map = new SDConcurrencyHashMap<String, String>(16, 0.75);
        map.put("1", "2");
        if(map.containsKey("3")){
            System.out.println("Cool!");
        }else{
            System.out.println("Cooler!");
        }
    }
}
