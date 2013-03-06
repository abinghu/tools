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

		std::cout << "�ѿ���:"
			<< formatTime.tm_mday << "��"
			<< formatTime.tm_hour << "ʱ"
			<< formatTime.tm_min << "��"
			<< formatTime.tm_sec << "��";
		std::cout << "\r";

		m_timer.expires_at(m_timer.expires_at()+boost::posix_time::seconds(1));
		m_timer.async_wait(boost::bind(&printer::printTime, this));
	}
protected:
	// ������Ϊ��λ��ʱ���ʽ��Ϊtm�ṹ
	struct tm FormatTime(DWORD liveTime)
	{
		struct tm formatTime;
		ZeroMemory(&formatTime, sizeof(formatTime));

		// ��
		formatTime.tm_sec = liveTime%60;
		if (liveTime/60 == 0)
			return formatTime;
		else
			liveTime = liveTime / 60;

		// ��
		formatTime.tm_min = liveTime%60;
		if (liveTime/60 == 0)
			return formatTime;
		else
			liveTime = liveTime / 60;

		// ʱ
		formatTime.tm_hour = liveTime%24;
		if (liveTime/24 == 0)
			return formatTime;
		else 
			liveTime = liveTime / 24;

		// �գ������ܳ���һ���°�
		formatTime.tm_mday = liveTime%30;

		return formatTime;
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
		<< sysStartTime.tm_sec << "\n\n";

	// ʵʱ��ʾ��ǰʱ��
	boost::asio::io_service io;
	printer printTime(io);
	io.run();

	return 0;
}
