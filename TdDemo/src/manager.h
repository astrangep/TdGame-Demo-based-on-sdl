#ifndef _MANAGER_H
#define _MANAGER_H
template <typename T>
class Manager 
{
public:
	static T* instance()
	{
		if (!manager) {
			manager = new T();
		}
		return manager;
		
	}

private:
	static T* manager;
protected:
	Manager() {}
	~Manager() {}
	Manager(const Manager&) = delete;
	Manager& operator=(const Manager&) = delete;
};
template <typename T>
T* Manager<T>::manager = nullptr;
#endif