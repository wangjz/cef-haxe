#ifndef MYQUEUE_H_
#define MYQUEUE_H_

#include <windows.h>

const int QUEUESIZE = 20;

struct MyMutex
{
   MyMutex()
   {
      mValid = true;
     // #ifdef HX_WINRT
     // InitializeCriticalSectionEx(&mCritSec,4000,0);
     // #else
      InitializeCriticalSection(&mCritSec);
      //#endif
   }
   ~MyMutex() { if (mValid) DeleteCriticalSection(&mCritSec); }
   void Lock() { EnterCriticalSection(&mCritSec); }
   void Unlock() { LeaveCriticalSection(&mCritSec); }
   BOOL TryLock() { return TryEnterCriticalSection(&mCritSec); }
   void Clean()
   {
      if (mValid)
      {
         DeleteCriticalSection(&mCritSec);
         mValid = false;
      }
   }

   bool             mValid;
   CRITICAL_SECTION mCritSec;
};


struct IOData{
	const char* mime;
	const char* redirectUrl;
	int status;
	unsigned char * bytes;
	bool isCancel;
	int bytesLen;
	IOData(){
		mime=NULL;
		bytes=NULL;
		status=0;
		redirectUrl=NULL;
		isCancel=false;
		bytesLen=0;
	}

	~IOData(){ 
		bytes=NULL;
		mime=NULL;
		redirectUrl=NULL;
	}
};

struct QueueData{
	IOData* data;
	const char* url;
	QueueData(){
		data=NULL;
		url=NULL;
	}

	~QueueData(){
		if(data!=NULL)delete data;
		data=NULL;
		url=NULL;
	}
};

//ÀàÄ£ÐÍ--------------------------------------
class  MyQueue
{
public:
	MyQueue(){
		mValid=true;
		rear=front=0;
		size=QUEUESIZE;
	}
	~MyQueue(){
		mValid=false;
		mMutex.Clean();
	}
   // public:
	int Push(QueueData* key){
		if(mValid==false)return 0;
		mMutex.Lock();
		if(IsFull())
		{
			//cout << "queue is full!" << endl;
			mMutex.Unlock();
			return 1;
		}            
		list[rear]=key;
		//printf("key is null:%d",key==NULL);
		//printf("url:%s:",key->url);
		//printf("in queue\n");
		rear =(rear+1)%size;
		mMutex.Unlock();
		return 2;
	}
	QueueData* Find(const char* key){
	   if(mValid==false)return NULL;
	   QueueData* temp=new QueueData();;
	   mMutex.Lock();
	   if(IsEmpty())
		{
			//printf("the queue is empty!\n");
			//cout << "the queue is empty!"<< endl;
			mMutex.Unlock();
			return temp;
		}
		int start=front;
		int end=rear;
		if(front>rear){
			end=QUEUESIZE+front;
		}
		if(start==end){
			mMutex.Unlock();
			return temp;
		}
		//printf("start:%d\n",start);
		//printf("end:%d\n",end);
		int _inx=-1;
		for(int i=start;i<end;++i){
			int k=i%QUEUESIZE;
			QueueData* data=list[k];
			//printf("data is null: %d\n",data!=NULL);
			//printf("data->url is null: %d\n",data->url!=NULL);
			if(data!=NULL&&data->url!=NULL){
				if(::strcmp(data->url,key)==0){ //find
					//printf("find\n");
					_inx=k;
					break;
				}
			}
		}
		if(_inx!=-1){
			if(_inx==front){ //head
				delete temp;
				temp=list[front];  
				front=(front+1)%size;
			}else{//swap
				 QueueData* t_swap=list[front];
				 QueueData* t_out=list[_inx];
				 delete temp;
				 temp=t_out;
				 //list[front]=t_out;
				 list[_inx]=t_swap;
				 front=(front+1)%size;
			}	
		}
		mMutex.Unlock();
		return temp;
	}
	QueueData* Pop(){
		if(mValid==false)return NULL;
		QueueData* temp=new QueueData();//=new QueueData();
		mMutex.Lock();
		if(IsEmpty())
		{
			//cout << "the queue is empty!"<< endl;
			mMutex.Unlock();
			return temp;
		}
		delete temp;
		temp=list[front];
		//temp=list[front]; 
		front=(front+1)%size;
		mMutex.Unlock();
		return temp;
	}
	int GetSize(){
		if(mValid==false)return 0;
		int realSize;
		mMutex.Lock();
		realSize=(rear-front)%size;
		mMutex.Unlock();
		return realSize;
	}
	bool IsFull(){
		if(mValid==false)return true;
		if((rear+1)%size==front)
			return true;
		else 
			return false;    
	}
	bool IsEmpty(){
		if(mValid==false)return false;
		if(front==rear)
			return true;
		else 
			return false;
	}
    //private:
    int rear;
    int front;
    int size;
	volatile bool mValid;
    QueueData* list[QUEUESIZE];
    MyMutex         mMutex;  
};
#endif