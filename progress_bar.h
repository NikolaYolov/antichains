#ifndef _PROGRESS_BAR_H_
#define _PROGRESS_BAR_H_

#include <algorithm> // for the min algorithm
#include <iostream>

using namespace::std;

class ProgressBar {
public:
	ProgressBar(size_t max_val)
		: max_(max_val)
		, step_(max_ / min(size_t(20000), max_))
		, last_progress_(0)
		, did_print_(false) {
	}

	void RegisterProgress() {
		Display_(last_progress_ + 1);
	}

	void SetProgress(size_t cur) {
		Display_(cur);
	}

	size_t Max() const {
		return max_;
	}

	~ProgressBar() {
		if (did_print_) {
			cout << endl;
		}
	}

private:
	void Display_(size_t new_progress) {
		if ((new_progress / step_) > (last_progress_ / step_)
		    || new_progress == max_) {
			size_t percent = (new_progress * 100) / max_;
			cout << "[";
			for (size_t i = 0; i < percent; ++i) {
				cout << "#";
			}
			for (size_t i = percent; i < 100; ++i) {
				cout << "-";
			}
			cout << "] " << percent << "% (" << new_progress << "/"
			     << max_ << ")\r";
			cout.flush();
			did_print_ = true;
		}
		last_progress_ = new_progress;
	}
	size_t max_;
	size_t step_;
	size_t last_progress_;
	bool did_print_;
};

#endif // _PROGRESS_BAR_H_
