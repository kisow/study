#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/managed_xsi_shared_memory.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/sync/sharable_lock.hpp>
#include <boost/interprocess/sync/interprocess_upgradable_mutex.hpp>
#include <boost/interprocess/sync/named_upgradable_mutex.hpp>
#include <functional>
#include <utility>
#include <iostream>
#include <vector>
#include <cassert>

using namespace std;
using namespace boost::interprocess;

template <typename T>
struct up
{
	typedef std::unique_ptr<T> type;
};

#define PRINT (cout << getpid() << " : " << __LINE__ << " : ")

template <typename KeyType>
class SharedMemoryCache 
{
public:
	typedef boost::interprocess::managed_xsi_shared_memory::segment_manager SegmentManager;
	typedef boost::interprocess::allocator<char, SegmentManager> CharAllocator;
	typedef boost::interprocess::basic_string<char, std::char_traits<char>, CharAllocator> ShmemString;
	typedef boost::interprocess::allocator<ShmemString, SegmentManager> ShmemStringAllocator;
	typedef ShmemString  ValueType;
	typedef std::pair<const KeyType, ValueType> PairType;
	typedef boost::interprocess::allocator<void, SegmentManager> VoidAllocator;
	typedef boost::interprocess::allocator<PairType, SegmentManager> ShmemPairAllocator;
	typedef boost::interprocess::map<KeyType, ValueType, std::less<KeyType>, ShmemPairAllocator> ShmemMap;
	typedef boost::interprocess::interprocess_upgradable_mutex SharableMutex;
	typedef boost::interprocess::scoped_lock<SharableMutex> ScopedLock;
	typedef boost::interprocess::sharable_lock<SharableMutex> SharedLock;

	static bool destroy(const xsi_key& shm_key)
	{
		try {
			xsi_shared_memory xsi(open_only, shm_key);
			xsi_shared_memory::remove(xsi.get_shmid());

			return true;	// found & destory
		} catch(interprocess_exception &e) {
			if(e.get_error_code() != not_found_error) throw;

			return false;	// not found
		}
	}

public:
	SharedMemoryCache(const xsi_key& shm_key, size_t memory_limit)
	{
		sgmt_.reset(new managed_xsi_shared_memory(open_or_create, shm_key, memory_limit));
		alloc_.reset(new VoidAllocator(sgmt_->get_segment_manager()));
		mutex_ = sgmt_->find_or_construct<SharableMutex>("SharableMutex")();
		map_ = sgmt_->find_or_construct<ShmemMap>("ShmemMap")(std::less<KeyType>(), (*alloc_));
	}

	size_t getFreeMemory() const { return sgmt_->get_free_memory(); }
	void printInfo() const 
	{
		cout << "total size : " << sgmt_->get_size() << endl;
		cout << "free size : " << getFreeMemory() << endl;
		cout << "min size : " << SegmentManager::segment_manager_base_type::get_min_size(1) << endl;
		cout << "key count : " << map_->size() << endl;
	}

	void put(const KeyType& key, const std::string& value)
	{
		ScopedLock lock(*mutex_);

		auto it = map_->find(key);
		if(it == map_->end()) {
			do {
				try {
					map_->emplace(key, ShmemString(value.c_str(), *alloc_));
				} catch (boost::interprocess::bad_alloc& ex) {
					pop();
					continue;
				}

				break;
			} while(1);
		}
	}

	bool get(const KeyType& key, std::string& value)
	{
		ScopedLock lock(*mutex_);

		auto it = map_->find(key);
		if(it != map_->end()) {
			value.assign(it->second.c_str(), it->second.length());

			return true;
		} else {
			return false;
		}
		return false;
	}

private:
	bool pop()
	{
		if(map_->empty() == false) {
			const auto victim_it = map_->begin();
			if(victim_it != map_->end()) map_->erase(victim_it);
		}
	}

private:
	up<managed_xsi_shared_memory>::type	sgmt_;
	up<VoidAllocator>::type	alloc_;
	ShmemMap*	map_;
	SharableMutex*	mutex_;
};
