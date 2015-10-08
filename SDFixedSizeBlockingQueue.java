import java.util.LinkedList;
import java.util.List;
import java.util.Queue;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

/**
 * @author amaliujia
 */
public class SDFixedSizeBlockingQueue<T> implements FixedSizeBlockingQueue<T>{
    private int capacity;
    private Queue<T> queue;
    private Lock lock = new ReentrantLock();
    private Condition notFull = this.lock.newCondition();
    private Condition notEmpty = this.lock.newCondition();
    private Lock listLock = new ReentrantLock();

    @Override
    public void init(int capacity) throws Exception {
        this.lock.lock();
        try{
            if (queue == null) {
                this.queue = new LinkedList<T>();
                this.capacity = capacity;
            }else{
                throw new Exception("Init queue more than once");
            }
        }finally {
            this.lock.unlock();
        }
    }

    @Override
    public void push(T obj) throws Exception {
        this.lock.lock();
        try{
            while (this.queue.size() == this.capacity){
                this.notFull.wait();
            }
            this.queue.offer(obj);
            this.notEmpty.notifyAll();
        }finally {
            this.lock.unlock();
        }
    }

    @Override
    public T pop() throws Exception{
        this.lock.lock();
        try{
            while (this.queue.size() == 0){
                this.notEmpty.wait();
            }
            T obj = this.queue.poll();
            this.notFull.notifyAll();
            return obj;
        }finally {
            this.lock.unlock();
        }
    }

    @Override
    public void pushList(List<T> objects) throws Exception {
        this.listLock.lock();
        this.lock.lock();

        try {
            for (T obj : objects) {
                while (this.queue.size() == this.capacity)
                    this.notFull.wait();
                this.queue.offer(obj);
                this.notEmpty.notifyAll();
            }
        }finally {
            this.lock.unlock();
            this.listLock.unlock();
        }
    }
}
