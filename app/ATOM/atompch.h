//
// Created by toor on 11/7/23.
//

#ifndef ATOM_ATOMPCH_H
#define ATOM_ATOMPCH_H
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <assert.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <algorithm>
#include <string>
#include <random>
#include <chrono>
#include <thread>
#include <mutex>
#include <queue>
#include <map>
#include <cctype>

#include "ATOM/Core/Editor/stb_image.h"

#include "ATOM/Core/Layers/Layer.h"
#include "ATOM/Core/Layers/LayerStack.h"


#include "glm/glm.hpp"

#include "ATOM/Core/macros.h"
#include "ATOM/Core/Loging/Log.h"


#include "imgui.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>
#ifndef STEAMNETWORKINGSOCKETS_OPENSOURCE
#include <steam/steam_api.h>
#endif

#ifdef _WIN32
#include <windows.h> // Ug, for NukeProcess -- see below
#else
#include <unistd.h>
#include <signal.h>
#endif


typedef struct {
    float pixelFromCenter;
    float confidence;
    std::string label;


}Sign;





#endif //ATOM_ATOMPCH_H
