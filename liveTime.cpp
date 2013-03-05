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
		std::cout << time.tm_mon+1 << "��"
			<< time.tm_mday << "��"
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
	// ��õ�ǰʱ��
	time_t	now;
	time(&now);

	// �����������ʱ��
	unsigned long tickCount = GetTickCount();
	struct tm sysStartTime;
	time_t startTime = (time_t)(now-tickCount/1000);
	localtime_s(&sysStartTime, &startTime);
    std::cout << "���������ʱ�䣺"
        << "\n  "
		<< sysStartTime.tm_year+1900 << "��"
		<< sysStartTime.tm_mon+1 << "��"
		<< sysStartTime.tm_mday << "��"
		<< "  "
		<< sysStartTime.tm_hour << ":"
		<< sysStartTime.tm_min << ":"
		<< sysStartTime.tm_sec << std::endl;

	// ʵʱ��ʾ��ǰʱ��
	boost::asio::io_service io;
	printer printTime(io);
	io.run();

	return 0;
}
