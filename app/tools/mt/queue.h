/// @file queue.h
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#pragma once

#include <deque>
#include <ostream>

#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_guard.hpp>
#include <boost/thread/lock_types.hpp>
#include <boost/thread/condition_variable.hpp>


namespace alexen {
namespace tools {
namespace mt {


template< typename T, typename BasedContainer = std::deque< T > >
class BlockingQueue
{
public:
     BlockingQueue() = default;

     void push( T value )
     {
          boost::lock_guard< boost::mutex > lock{ m_ };
          queue_.push_back( std::move( value ) );
          cv_.notify_one();
     }

     T pop()
     {
          boost::unique_lock< boost::mutex > lock{ m_ };
          if( empty() )
          {
               cv_.wait( lock, [ this ]{ return !empty(); } );
          }
          T value = queue_.front();
          queue_.pop_front();
          return value;
     }

     void poke() const noexcept
     {
          cv_.notify_all();
     }

     std::ostream& print( std::ostream& os, const char* const sep = ", " ) const
     {
          boost::lock_guard< boost::mutex > lock{ m_ };
          const char* sep_ = "";
          for( auto&& each: queue_ )
          {
               os << sep_ << each;
               sep_ = sep;
          }
          return os;
     }

private:
     bool empty() const noexcept
     {
          return queue_.empty();
     }

     mutable boost::mutex m_;
     mutable boost::condition_variable cv_;
     mutable BasedContainer queue_;
};


template< typename T >
inline std::ostream& operator<<( std::ostream& os, const BlockingQueue< T >& q )
{
     return q.print( os );
}



} // namespace mt
} // namespace tools
} // namespace alexen
