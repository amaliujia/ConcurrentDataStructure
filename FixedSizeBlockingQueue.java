import java.util.List;

/**
 * @author amaliujia
 */
public interface FixedSizeBlockingQueue<T> {
    
    // only initialize the queue once and throws exception if user
    // try to initialize it multi times.
    void init(int capacity) throws Exception;

    // Push a object of T into queue, throws exception if queue is not
    // initialized
    void push(T obj) throws Exception;

    // Pop a object of T from queue, throws exception if queue is not
    // initialized
    T pop()throws Exception;

    void pushList(List<T> objects) throws Exception;
}
