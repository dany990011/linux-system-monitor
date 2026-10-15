#pragma once
#include "SystemSnapshot.hpp"


std::string stringSerializeSnapshot(SystemSnapshot snapshot);

SystemSnapshot deSerializeSnapshot(const std::string& data);