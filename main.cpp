#include <iostream>
using std::cout;
using std::endl;

#include <map>
using std::map;

#include <sstream>
using std::istringstream;
using std::ostringstream;

#include <vector>
using std::vector;

#include <list>
using std::list;

#include <string>
using std::string;
using std::getline;
using std::to_string;

#include <format>
using std::format;

#include <chrono>

#include <iomanip>
using std::get_time;
using std::put_time;


bool date_less_than(const tm& left, const tm& right)
{
	if ((right.tm_year + 1900) > (left.tm_year + 1900))
		return true;
	else if ((right.tm_year + 1900) < (left.tm_year + 1900))
		return false;

	if (right.tm_mon > left.tm_mon)
		return true;
	else if (right.tm_mon < left.tm_mon)
		return false;

	if (right.tm_mday > left.tm_mday)
		return true;
	else if (right.tm_mday < left.tm_mday)
		return false;

	return false;
}

class row
{
public:
	bool operator<(const row& right) const
	{
		if (right.id > id)
			return true;
		else if (right.id < id)
			return false;

		if (right.code > code)
			return true;
		else if (right.code < code)
			return false;

		tm ta = {}, tb = {};

		istringstream iss;

		iss.clear();
		iss.str(datetime_begin);
		iss >> get_time(&ta, "%d%b%Y");

		iss.clear();
		iss.str(right.datetime_begin);
		iss >> get_time(&tb, "%d%b%Y");

		if (date_less_than(ta, tb))
			return true;
		else if (date_less_than(tb, ta))
			return false;

		return false;
	}

	bool operator==(const row& right) const
	{
		if (id != right.id)
			return false;

		if (datetime_begin != right.datetime_begin)
			return false;

		if (code != right.code)
			return false;

		return true;
	}

	size_t id;
	string datetime_begin;
	string code;
};

void sort_by_id(vector<row>& vn)
{
	map<size_t, vector<row>> mv;

	for (size_t i = 0; i < vn.size(); i++)
		mv[vn[i].id].push_back(vn[i]);

	vn.clear();

	for (map<size_t, vector<row>>::iterator i = mv.begin(); i != mv.end(); i++)
		sort(i->second.begin(), i->second.end());

	for (map<size_t, vector<row>>::iterator i = mv.begin(); i != mv.end(); i++)
		for (size_t j = 0; j < i->second.size(); j++)
			vn.push_back(i->second[j]);

	mv.clear();
}

void get_rand_string(string& s, size_t num_chars)
{
	s.clear();

	for(size_t i = 0; i < num_chars; i++)
		s.push_back(rand() % 255);
}


const vector<string> months = { "Jan", "Feb", "Mar" , "Apr" , "May" , "Jun" , "Jul" , "Aug" , "Sep" , "Oct" , "Nov" , "Dec" };


int main(void)
{
	std::chrono::high_resolution_clock::time_point start_time, end_time;
	std::chrono::duration<float, std::milli> elapsed;
	vector<row> vn;
	const size_t limit = 10000000;

	for (size_t i = 0; i < limit; i++)
	{
		if (i % 100000 == 0)
			cout << i / float(limit) << endl;

		row n;
		n.id = rand()%1000;
		get_rand_string(n.code, 10);
		n.datetime_begin = to_string(rand()%31 + 1) + months[rand()%12] + to_string(rand() % 100 + 2000);

		vn.push_back(n);
	}

	vector<row> vn_backup = vn;

	start_time = std::chrono::high_resolution_clock::now();
	sort(vn.begin(), vn.end());
	end_time = std::chrono::high_resolution_clock::now();

	elapsed = end_time - start_time;
	cout << "std::sort() duration: " << elapsed.count() / 1000.0f << " seconds" << endl;

	vector<row> vn_a_finished = vn;

	
	vn = vn_backup;

	start_time = std::chrono::high_resolution_clock::now();
	sort_by_id(vn);
	end_time = std::chrono::high_resolution_clock::now();

	elapsed = end_time - start_time;
	cout << "bucket sort duration: " << elapsed.count() / 1000.0f << " seconds" << endl;

	vector<row> vn_b_finished = vn;

	for (size_t i = 0; i < vn_a_finished.size(); i++)
	{
		bool same = (vn_a_finished[i] == vn_b_finished[i]);

		if (false == same)
		{
			cout << "False" << endl;
			cout << vn_a_finished[i].id << " " << vn_b_finished[i].id << endl;
			cout << vn_a_finished[i].code << " " << vn_b_finished[i].code << endl;
			cout << vn_a_finished[i].datetime_begin << " " << vn_b_finished[i].datetime_begin << endl;
			cout << endl;
		}
	}

	return 0;
}