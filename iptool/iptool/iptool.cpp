#include "./iptools/core.h"
#include <string.h>
#include <fstream>

using namespace std;

#define MAXLEN 256

int main (int argc, char** argv)
{
	ifstream fp(argv[1]);
	char str[MAXLEN];
	char infile[MAXLEN];
	char outfile[MAXLEN];
	int nOP;
	int roi, x, y, Sx, Sy;
	if (!fp.is_open()) {
		fprintf(stderr, "Can't open file: %s\n", argv[1]);
		exit(1);
	}

	fp >> nOP;
	for (int OP = 0; OP < nOP; OP++) {
		fp >> str;
		strcpy_s(infile, MAXLEN, str);

		fp >> str;
		strcpy_s(outfile, MAXLEN, str);

		fp >> str;
		if (strncmp(str, "opencv", 6) == 0) {
			cv::Mat src = cv::imread(infile);
			cv::Mat tgt;
			if (src.empty()) {
				cout << "Could not open or find the image: " << infile << endl;
				return -1;
			}

			fp >> str;
			if (strncmp(str, "gray", 4) == 0) {
				utility::cv_gray(src, tgt);
			}
			else if (strncmp(str, "blur_avg", 8) == 0) {
				int windowSize;
				fp >> windowSize;
				utility::cv_avgblur(src, tgt, windowSize);
			}
			else if (strncmp(str, "equalization", 12) == 0) {
				fp >> roi;
				if (roi < 4 && roi >= 0) {
					if (roi == 0) {
						utility::cv_histEqual(src, tgt, 0, 0, 0, 0, 0);
					}
					else {
						for (int i = 1; i <= roi; i++) {
							fp >> x >> y >> Sx >> Sy;
							utility::cv_histEqual(src, tgt, x, y, Sx, Sy, i);
						}
					}
				}
				else {
					cout << "ROI needs to be from 0 to 3 inclusive" << endl;
				}
			}
			else if (strncmp(str, "sobel", 5) == 0) {
				fp >> roi;
				if (roi <= 4 && roi >= 0) {
					if (roi == 0) {
						utility::cv_sobel(src, tgt, 0, 0, 0, 0, 0);
					}
					else if (roi == 4) {
						utility::cv_histEqual(src, tgt, 0, 0, 0, 0, 0);
						utility::cv_sobel(tgt, tgt, 0, 0, 0, 0, 0);
					}
					else {
						for (int i = 1; i <= roi; i++) {
							fp >> x >> y >> Sx >> Sy;
							utility::cv_sobel(src, tgt, x, y, Sx, Sy, i);
						}
					}
				}
				else {
					cout << "ROI needs to be from 0 to 3 inclusive" << endl;
				}
			}
			else if (strncmp(str, "canny", 5) == 0) {
				fp >> roi;
				if (roi <= 4 && roi >= 0) {
					if (roi == 0) {
						utility::cv_canny(src, tgt, 0, 0, 0, 0, 0);
					}
					else if (roi == 4) {
						utility::cv_histEqual(src, tgt, 0, 0, 0, 0, 0);
						utility::cv_canny(tgt, tgt, 0, 0, 0, 0, 0);
					}
					else {
						for (int i = 1; i <= roi; i++) {
							fp >> x >> y >> Sx >> Sy;
							utility::cv_canny(src, tgt, x, y, Sx, Sy, i);
						}
					}
				}
				else {
					cout << "ROI needs to be from 0 to 3 inclusive" << endl;
				}
			}
			else {
				printf("No function: %s\n", str);
				continue;
			}

			cv::imwrite(outfile, tgt);
		}
		else {
			image src, tgt;
			src.read(infile);
			if (strncmp(str, "add", 3) == 0) {
				/* Add Intensity */
				fp >> str;
				utility::addGrey(src, tgt, atoi(str));
			}

			else if (strncmp(str, "binarize", 8) == 0) {
				/* Thresholding */
				fp >> str;
				utility::binarize(src, tgt, atoi(str));
			}

			else if (strncmp(str, "scale", 5) == 0) {
				/* Image scaling */
				fp >> str;
				utility::scale(src, tgt, atof(str));
			}

			else {
				printf("No function: %s\n", str);
				continue;
			}

			tgt.save(outfile);
		}
	}
	//fclose(fp);
	fp.close();
	return 0;
}

