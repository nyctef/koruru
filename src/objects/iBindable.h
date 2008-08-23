#ifndef IBINDABLE_H_
#define IBINDABLE_H_

/** Provides a common interface for cTexture and cFrameBuffer objects.
 */
class iBindable {
public:
	iBindable();
	virtual ~iBindable();
	
	virtual void bind() = 0;
	virtual void unbind() = 0;
	
	int width, height;
};

#endif /*IBINDABLE_H_*/
