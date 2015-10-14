#ifndef _SOUND_H_
#define _SOUND_H_


class Sound {
public:
	Sound()
	{
		
	}
	
	~Sound()
	{
		Release();
	}
	
	
	void Create(int fmt, int bytes, int dupnum = 1)
	{
		
	}
	
	void Release()
	{
	}
	
	
	void Fill(void *data, int bytes)
	{
		
	}
	
	
	void Play(int type)
	{
	}
	
	void SetPitch(int pitch)
	{
	}
	
	void Stop(int type = 0)
	{
	}
	
	void SetPos(float pos[3])
	{
		
	}
	
};


#endif //_SOUND_H_

