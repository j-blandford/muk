#define LIBCXXRT_WEAK_LOCKS 

/* Single execution handling.  */
#define PTHREAD_ONCE_INIT 0

typedef unsigned int pthread_key_t;
typedef int pthread_once_t;
typedef long long int __pthread_cond_align_t;
typedef unsigned long int pthread_t;

struct _pthread_fastlock
{
  long int __status;
  int __spinlock;
};

typedef struct _pthread_descr_struct *_pthread_descr;

struct pthread_mutex_t {
  int __m_reserved;
  int __m_count;
  _pthread_descr __m_owner;
  int __m_kind;
  struct _pthread_fastlock __m_lock;
};

typedef struct
{
  int __mutexkind;
}
pthread_mutexattr_t;

typedef struct
{
  struct _pthread_fastlock __c_lock;
  _pthread_descr __c_waiting;
  char __padding[48 - sizeof (struct _pthread_fastlock) -
		 sizeof (_pthread_descr) - sizeof (__pthread_cond_align_t)];
  __pthread_cond_align_t __align;
}
pthread_cond_t;
typedef struct
{
  int __dummy;
}
pthread_condattr_t;

typedef struct _pthread_rwlock_t
{
  struct _pthread_fastlock __rw_lock;
  int __rw_readers;
  _pthread_descr __rw_writer;
  _pthread_descr __rw_read_waiting;
  _pthread_descr __rw_write_waiting;
  int __rw_kind;
  int __rw_pshared;
}
pthread_rwlock_t;
typedef struct
{
  int __lockkind;
  int __pshared;
}
pthread_rwlockattr_t;

# define __PTHREAD_SPINS             0
# define PTHREAD_MUTEX_INITIALIZER \
   { 0, 0, 0, 0, 0, __PTHREAD_SPINS, 0, 0 } 

/* Conditional variable handling.  */
#define PTHREAD_COND_INITIALIZER { 0, 0, 0, 0, 0, (void *) 0, 0, 0 }

namespace { void* threadDataTable[64]; int freeEntry = 0;}
int pthread_key_create(pthread_key_t* key, void (*)(void*));
int pthread_once(pthread_once_t* control, void (*init)(void));
 
void* pthread_getspecific(pthread_key_t key) {
	return threadDataTable[key];
}
 
int pthread_setspecific(pthread_key_t key, const void* data) ;
int pthread_mutex_init(pthread_mutex_t* mutex, const pthread_mutexattr_t*);
 
int pthread_mutex_lock(pthread_mutex_t* mutex);
 
int pthread_mutex_unlock(pthread_mutex_t* mutex);
int pthread_cond_wait(pthread_cond_t*, pthread_mutex_t*);
 
int pthread_cond_signal(pthread_cond_t*) ;