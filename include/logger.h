#ifndef _EZNETPP_SRC_LOGGER_H_
#define _EZNETPP_SRC_LOGGER_H_

#include <mutex>

#include "macros.h"

namespace eznetpp {
class logger {
public:
	// log level
	typedef enum _log_level {
		fatal	= 0,
		error,
		warn,
		info,
		debug,

		max_depth
	} log_level;

	static logger& instance();

	void log(log_level level, const std::string& msg, const std::string& file = ""
					, unsigned int line = 0, const std::string& func = "");
	
protected:
	friend class cleanup;
	class cleanup {
  public:
		~cleanup();
	};

	static logger* inst;


private:
	logger(void);
	virtual ~logger(void);
  DISALLOW_COPY_AND_ASSIGN(logger);

	static std::mutex log_mutex;
};

} // namespace eznetpp

#endif // _EZNETPP_SRC_LOGGER_H_
