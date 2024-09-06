#ifndef REQUESTREADER_HPP
# define REQUESTREADER_HPP

# include <string>
# include <iostream>
# include <map>
# include <cstdlib>
# include <cstring>
# include <sys/socket.h>
# include <string>
# include <typeinfo>
# include <vector>
# include <fstream>
# include <algorithm>
#define CRLF			"\r\n"

class RequestReader
{

	public:
		RequestReader();
		~RequestReader();

		bool								readHttpRequest(int &fdConection);

		// GETTERS
		std::string							getMethod(void) const;
		std::string							getHttpVersion(void) const;
		std::string							getBody(void) const;
		std::vector<char>					getRawBody(void) const;
		std::string							getHeader(std::string headerName) const;
		std::string							getFullRequest(void) const;
		int									getContentLength(void) const;
		std::string							getRequestedRoute(void) const;
		std::vector<std::string>			getMultipartHeaders(void) const;
		std::vector<std::string>			getMultipartValues(void) const;
	private:

		// READ START LINE
		void								readRequestStartLine(void);

		// READ REQUEST HEADER
		void								readRequestHeader(void);

		// READ REQUEST BODY
		void								readRequestBody(void);
		void								readRequestBodyChunked(void);

		void								readRequestBodyChunkedVec(void);
		void								readChunk(int fd, std::size_t limit, bool &error);

		size_t								readChunkSize();
		void								readRequestBodyMultipart(void);
		void								readMultipartInfoChunked(const std::string& boundary, std::vector<char> &tempLine);
		void								readMultipartInfo(const std::string& boundary, std::string &tempLine);

		// READLINE AND UTILS
		void								readLine(int fd, std::string &line, std::string delimiter, bool &error);
		void								readLineBody(int fd, std::string &line, int contentLength, bool &error);
		std::string							intToString(int value);
		bool								isDelimiter(std::string line, std::string delimiter);

		// DEBUG
		void								printHeaderDataStructure(void);

		// STATUS VARS
		bool									_errorRead;
		bool									_incompleted;
		bool									_readRawBody;

		// REQUEST VARS
		std::map<std::string, std::string> 		_headers;
		std::vector<std::string>				_multipartHeaders;
		std::vector<std::string>				_multipartValues;
		std::string								_method;
		std::string								_requestedRoute;
		std::string								_httpVersion;
		std::vector<char>						_requestBody;
		int										_fdClient;
		std::string								_fullRequest;
		std::vector<char>						_rawBody;
};

#endif
