#include "include/HospitalManagement.h"

int main() {
    HospitalManagement& hospitalManagement = HospitalManagement::create();
    hospitalManagement.run();
    return 0;
}