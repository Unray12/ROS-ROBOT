// Generated by gencpp from file unity_robotics_demo_msgs/PositionService.msg
// DO NOT EDIT!


#ifndef UNITY_ROBOTICS_DEMO_MSGS_MESSAGE_POSITIONSERVICE_H
#define UNITY_ROBOTICS_DEMO_MSGS_MESSAGE_POSITIONSERVICE_H

#include <ros/service_traits.h>


#include <unity_robotics_demo_msgs/PositionServiceRequest.h>
#include <unity_robotics_demo_msgs/PositionServiceResponse.h>


namespace unity_robotics_demo_msgs
{

struct PositionService
{

typedef PositionServiceRequest Request;
typedef PositionServiceResponse Response;
Request request;
Response response;

typedef Request RequestType;
typedef Response ResponseType;

}; // struct PositionService
} // namespace unity_robotics_demo_msgs


namespace ros
{
namespace service_traits
{


template<>
struct MD5Sum< ::unity_robotics_demo_msgs::PositionService > {
  static const char* value()
  {
    return "5190f887b70decc2fafe0ae2939b233a";
  }

  static const char* value(const ::unity_robotics_demo_msgs::PositionService&) { return value(); }
};

template<>
struct DataType< ::unity_robotics_demo_msgs::PositionService > {
  static const char* value()
  {
    return "unity_robotics_demo_msgs/PositionService";
  }

  static const char* value(const ::unity_robotics_demo_msgs::PositionService&) { return value(); }
};


// service_traits::MD5Sum< ::unity_robotics_demo_msgs::PositionServiceRequest> should match
// service_traits::MD5Sum< ::unity_robotics_demo_msgs::PositionService >
template<>
struct MD5Sum< ::unity_robotics_demo_msgs::PositionServiceRequest>
{
  static const char* value()
  {
    return MD5Sum< ::unity_robotics_demo_msgs::PositionService >::value();
  }
  static const char* value(const ::unity_robotics_demo_msgs::PositionServiceRequest&)
  {
    return value();
  }
};

// service_traits::DataType< ::unity_robotics_demo_msgs::PositionServiceRequest> should match
// service_traits::DataType< ::unity_robotics_demo_msgs::PositionService >
template<>
struct DataType< ::unity_robotics_demo_msgs::PositionServiceRequest>
{
  static const char* value()
  {
    return DataType< ::unity_robotics_demo_msgs::PositionService >::value();
  }
  static const char* value(const ::unity_robotics_demo_msgs::PositionServiceRequest&)
  {
    return value();
  }
};

// service_traits::MD5Sum< ::unity_robotics_demo_msgs::PositionServiceResponse> should match
// service_traits::MD5Sum< ::unity_robotics_demo_msgs::PositionService >
template<>
struct MD5Sum< ::unity_robotics_demo_msgs::PositionServiceResponse>
{
  static const char* value()
  {
    return MD5Sum< ::unity_robotics_demo_msgs::PositionService >::value();
  }
  static const char* value(const ::unity_robotics_demo_msgs::PositionServiceResponse&)
  {
    return value();
  }
};

// service_traits::DataType< ::unity_robotics_demo_msgs::PositionServiceResponse> should match
// service_traits::DataType< ::unity_robotics_demo_msgs::PositionService >
template<>
struct DataType< ::unity_robotics_demo_msgs::PositionServiceResponse>
{
  static const char* value()
  {
    return DataType< ::unity_robotics_demo_msgs::PositionService >::value();
  }
  static const char* value(const ::unity_robotics_demo_msgs::PositionServiceResponse&)
  {
    return value();
  }
};

} // namespace service_traits
} // namespace ros

#endif // UNITY_ROBOTICS_DEMO_MSGS_MESSAGE_POSITIONSERVICE_H
