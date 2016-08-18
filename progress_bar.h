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
		, cur_(0)
		, did_print_(false) {
	}

	void RegisterProgress() {
		cur_++;
		Display_();
	}

	void SetProgress(size_t cur) {
		cur_ = cur;
		Display_();
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
	void Display_() {
		if (cur_ % step_ == 0 || cur_ == max_) {
			size_t percent = (cur_ * 100) / max_;
			cout << "[";
			for (size_t i = 0; i < percent; ++i) {
				cout << "#";
			}
			for (size_t i = percent; i < 100; ++i) {
				cout << "-";
			}
			cout << "] " << percent << "% (" << cur_ << "/"
			     << max_ << ")\r";
			cout.flush();
			did_print_ = true;
		}
	}
	size_t max_;
	size_t step_;
	size_t cur_;
	bool did_print_;
};

#endif // _PROGRESS_BAR_H_
