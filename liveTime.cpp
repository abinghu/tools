#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

class printer
{
public:
	printer(boost::asio::io_service& io)
		: m_timer(io, boost::posix_time::seconds(1))
	{
		m_timer.async_wait(boost::bind(&printer::printTime, this));
	}
    ~printer(){}
	void printTime()
	{
		time_t now;
		time(&now);
		struct tm time;
		localtime_s(&time, &now);
		std::cout << time.tm_mon+1 << "月"
			<< time.tm_mday << "日"
			<< "  "
			<< time.tm_hour << ":"
			<< time.tm_min << ":"
			<< time.tm_sec;
		std::cout << "\r";

		m_timer.expires_at(m_timer.expires_at()+boost::posix_time::seconds(1));
		m_timer.async_wait(boost::bind(&printer::printTime, this));
	}

private:
	boost::asio::deadline_timer m_timer;
};

int main(int argc, char *argv[])
{
	// 获得当前时间
	time_t	now;
	time(&now);

	// 计算电脑启动时间
	unsigned long tickCount = GetTickCount();
	struct tm sysStartTime;
	time_t startTime = (time_t)(now-tickCount/1000);
	localtime_s(&sysStartTime, &startTime);
    std::cout << "计算机启动时间："
        << "\n  "
		<< sysStartTime.tm_year+1900 << "年"
		<< sysStartTime.tm_mon+1 << "月"
		<< sysStartTime.tm_mday << "日"
		<< "  "
		<< sysStartTime.tm_hour << ":"
		<< sysStartTime.tm_min << ":"
		<< sysStartTime.tm_sec << std::endl;

	// 实时显示当前时间
	boost::asio::io_service io;
	printer printTime(io);
	io.run();

	return 0;
}
