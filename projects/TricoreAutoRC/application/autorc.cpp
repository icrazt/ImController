/*
wigets:
- [] plot wiget
- [] tf data 

*/
#include "imc_data.hpp"
#include "autorc.h"
#include "imgui.h"
#include "implot.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#ifndef PI
#define PI 3.14159265358979323846
#endif

AutoRC_DataStruct AutoCurrentStatus,AutoRC_TxData;

// utility structure for realtime plot
struct ScrollingBuffer {
    int MaxSize;
    int Offset;
    ImVector<ImVec2> Data;
    ScrollingBuffer(int max_size = 2000) {
        MaxSize = max_size;
        Offset = 0;
        Data.reserve(MaxSize);
    }
    void AddPoint(float x, float y) {
        if (Data.size() < MaxSize)
            Data.push_back(ImVec2(x, y));
        else {
            Data[Offset] = ImVec2(x, y);
            Offset = (Offset + 1) % MaxSize;
        }
    }
    void Erase() {
        if (Data.size() > 0) {
            Data.shrink(0);
            Offset = 0;
        }
    }
};

// utility structure for realtime plot
struct RollingBuffer {
    float Span;
    ImVector<ImVec2> Data;
    RollingBuffer() {
        Span = 10.0f;
        Data.reserve(2000);
    }
    void AddPoint(float x, float y) {
        float xmod = fmodf(x, Span);
        if (!Data.empty() && xmod < Data.back().x)
            Data.shrink(0);
        Data.push_back(ImVec2(xmod, y));
    }
};



void AutoRC_DataPlot(void)
{
	//ImGui::CreateContext();
	ImPlot::CreateContext();
    if (ImGui::CollapsingHeader("Realtime Plots")) {
        ImGui::BulletText("Move your mouse to change the data!");
        ImGui::BulletText("This example assumes 60 FPS. Higher FPS requires larger buffer size.");
        ImGui::Text("value:%.2f,%.2f", AutoCurrentStatus.setspeed[0], AutoCurrentStatus.motorspeed[0]);
        static ScrollingBuffer sdata1, sdata2;
        static RollingBuffer   rdata1, rdata2;
        ImVec2 mouse = ImGui::GetMousePos();
        static float t = 0;
        t += ImGui::GetIO().DeltaTime;
        sdata1.AddPoint(t, AutoCurrentStatus.setspeed[0]);
        sdata2.AddPoint(t, AutoCurrentStatus.motorspeed[0]);
        rdata1.AddPoint(t, mouse.x * 0.0005f);
        rdata2.AddPoint(t, mouse.y * 0.0005f);

        static float history = 10.0f;
        ImGui::SliderFloat("History", &history, 1, 30, "%.1f s");
        rdata1.Span = history;
        rdata2.Span = history;

        static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;
        ImPlot::SetNextPlotLimitsX(t - history, t, ImGuiCond_Always);
        if (ImPlot::BeginPlot("##Scrolling", NULL, NULL, ImVec2(-1, 300), 0, flags, flags)) {
            ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
            ImPlot::PlotShaded("Mouse X", &sdata1.Data[0].x, &sdata1.Data[0].y, sdata1.Data.size(), -INFINITY, sdata1.Offset, 2 * sizeof(float));
            ImPlot::PlotLine("Mouse Y", &sdata2.Data[0].x, &sdata2.Data[0].y, sdata2.Data.size(), sdata2.Offset, 2 * sizeof(float));
            ImPlot::EndPlot();
        }
        ImPlot::SetNextPlotLimitsX(0, history, ImGuiCond_Always);
        ImPlot::SetNextPlotLimitsY(0, 1);
        if (ImPlot::BeginPlot("##Rolling", NULL, NULL, ImVec2(-1, 150), 0, flags, flags)) {
            ImPlot::PlotLine("Mouse X", &rdata1.Data[0].x, &rdata1.Data[0].y, rdata1.Data.size(), 0, 2 * sizeof(float));
            ImPlot::PlotLine("Mouse Y", &rdata2.Data[0].x, &rdata2.Data[0].y, rdata2.Data.size(), 0, 2 * sizeof(float));
            ImPlot::EndPlot();
        }
    }
	ImPlot::DestroyContext();
	//ImGui::DestroyContext();
}