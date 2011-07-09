package mapnik;

/**
 * Base class for native objects
 * @author stella
 *
 */
public abstract class NativeObject {
	/**
	 * Native pointer.  Depending on the type, this will either be a pointer
	 * directly to the object or a smart pointer.
	 */
	long ptr;
	
	NativeObject() {
		Mapnik.incrementAlloc(getClass(), 1);
	}
	
	/**
	 * Manually disposes of the native resources associated with this
	 * object (versus waiting for finalization).
	 */
	public final void dispose() {
		if (ptr!=0) {
			long ptrLocal=ptr;
			ptr=0;
			dealloc(ptrLocal);
			Mapnik.incrementAlloc(getClass(), -1);
		}
	}
	
	/**
	 * @return true if the native resources associated with this object have been destroyed
	 */
	public final boolean isDisposed() {
		return ptr==0;
	}
	
	@Override
	protected final void finalize() throws Throwable {
		super.finalize();
		dispose();
	}
	
	/**
	 * Called on finalize and dispose to dealloc the pointer if != 0
	 * @param ptr
	 */
	abstract void dealloc(long ptr);
}
