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
		DWORD liveTime = GetTickCount()/1000;
		struct tm formatTime = FormatTime(liveTime);

		std::cout << "已开机:"
			<< formatTime.tm_mday << "日"
			<< formatTime.tm_hour << "时"
			<< formatTime.tm_min << "分"
			<< formatTime.tm_sec << "秒";
		std::cout << "\r";

		m_timer.expires_at(m_timer.expires_at()+boost::posix_time::seconds(1));
		m_timer.async_wait(boost::bind(&printer::printTime, this));
	}
protected:
	// 将以秒为单位的时间格式化为tm结构
	struct tm FormatTime(DWORD liveTime)
	{
		struct tm formatTime;
		ZeroMemory(&formatTime, sizeof(formatTime));

		// 秒
		formatTime.tm_sec = liveTime%60;
		if (liveTime/60 == 0)
			return formatTime;
		else
			liveTime = liveTime / 60;

		// 分
		formatTime.tm_min = liveTime%60;
		if (liveTime/60 == 0)
			return formatTime;
		else
			liveTime = liveTime / 60;

		// 时
		formatTime.tm_hour = liveTime%24;
		if (liveTime/24 == 0)
			return formatTime;
		else 
			liveTime = liveTime / 24;

		// 日，不可能超过一个月吧
		formatTime.tm_mday = liveTime%30;

		return formatTime;
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
		<< sysStartTime.tm_sec << "\n\n";

	// 实时显示当前时间
	boost::asio::io_service io;
	printer printTime(io);
	io.run();

	return 0;
}
