#include "Profiler.h"
#include "imgui.h"
#include <algorithm>

std::vector<Profiler::FrameData> Profiler::_history;
int Profiler::_selectedFrameIndex = -1;

void Profiler::AddFrameData(const FrameData& data)
{
	_history.push_back(data);
	if (_history.size() > _maxHistory)
	{
		_history.erase(_history.begin());
	}
	
	// 最新のフレームを選択状態にする（追従モードの場合などを考慮して適宜調整可能）
	if (_selectedFrameIndex == -1 || _selectedFrameIndex == (int)_history.size() - 2)
	{
		_selectedFrameIndex = (int)_history.size() - 1;
	}
}

void Profiler::Render()
{
	if (!ImGui::Begin("Profiler"))
	{
		ImGui::End();
		return;
	}

	if (_history.empty())
	{
		ImGui::Text("No data recorded yet.");
		ImGui::End();
		return;
	}

	// グラフの描画領域の設定
	float graphHeight = 150.0f;
	ImVec2 canvasPos = ImGui::GetCursorScreenPos();
	ImVec2 canvasSize = ImVec2(ImGui::GetContentRegionAvail().x, graphHeight);
	ImDrawList* drawList = ImGui::GetWindowDrawList();

	// 背景
	drawList->AddRectFilled(canvasPos, ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + canvasSize.y), IM_COL32(30, 30, 30, 255));

	float frameWidth = canvasSize.x / _maxHistory;
	float maxTime = 16.67f; // 60FPSを基準
	for (const auto& frame : _history)
	{
		maxTime = std::max(maxTime, frame.totalFrameTime);
	}
	maxTime *= 1.2f; // 上部に少し余裕を持たせる

	// グラフの描画（積み上げ棒グラフ形式）
	for (size_t i = 0; i < _history.size(); ++i)
	{
		const auto& frame = _history[i];
		float x = canvasPos.x + i * frameWidth;
		float yOffset = 0.0f;

		// 各カテゴリの色を定義
		auto getCategoryColor = [](const std::string& name) -> ImU32 {
			if (name == "Rendering") return IM_COL32(76, 175, 80, 255);  // Green
			if (name == "Scripts")   return IM_COL32(33, 150, 243, 255); // Blue
			if (name == "Physics")   return IM_COL32(255, 152, 0, 255);  // Orange
			return IM_COL32(158, 158, 158, 255);                        // Gray (Other)
		};

		for (const auto& pair : frame.categoryTimes)
		{
			float h = (pair.second / maxTime) * graphHeight;
			drawList->AddRectFilled(
				ImVec2(x, canvasPos.y + canvasSize.y - yOffset - h),
				ImVec2(x + frameWidth - 1.0f, canvasPos.y + canvasSize.y - yOffset),
				getCategoryColor(pair.first)
			);
			yOffset += h;
		}

		// 選択中のフレームをハイライト
		if ((int)i == _selectedFrameIndex)
		{
			drawList->AddRect(
				ImVec2(x - 1.0f, canvasPos.y),
				ImVec2(x + frameWidth + 1.0f, canvasPos.y + canvasSize.y),
				IM_COL32(255, 255, 255, 200)
			);
		}
	}

	// 60FPS (16.6ms) の基準線
	float line60fpsY = canvasPos.y + canvasSize.y - (16.67f / maxTime) * graphHeight;
	drawList->AddLine(ImVec2(canvasPos.x, line60fpsY), ImVec2(canvasPos.x + canvasSize.x, line60fpsY), IM_COL32(255, 255, 255, 50));

	// グラフのクリック操作
	ImGui::InvisibleButton("##ProfilerGraph", canvasSize);
	if (ImGui::IsItemHovered())
	{
		if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
		{
			float mouseX = ImGui::GetIO().MousePos.x - canvasPos.x;
			_selectedFrameIndex = (int)(mouseX / frameWidth);
			if (_selectedFrameIndex < 0) _selectedFrameIndex = 0;
			if (_selectedFrameIndex >= (int)_history.size()) _selectedFrameIndex = (int)_history.size() - 1;
		}
	}

	// 詳細表示
	if (_selectedFrameIndex >= 0 && _selectedFrameIndex < (int)_history.size())
	{
		const FrameData& selectedFrame = _history[_selectedFrameIndex];
		ImGui::Text("Selected Frame: %d | Total: %.2f ms (%.1f FPS)", _selectedFrameIndex, selectedFrame.totalFrameTime, 1000.0f / selectedFrame.totalFrameTime);
		
		if (ImGui::BeginTable("DetailedTiming", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
		{
			ImGui::TableSetupColumn("Category");
			ImGui::TableSetupColumn("Time (ms)");
			ImGui::TableSetupColumn("Percentage");
			ImGui::TableHeadersRow();

			for (const auto& pair : selectedFrame.categoryTimes)
			{
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("%s", pair.first.c_str());
				
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%.3f ms", pair.second);
				
				ImGui::TableSetColumnIndex(2);
				float pct = (pair.second / selectedFrame.totalFrameTime) * 100.0f;
				ImGui::Text("%.1f%%", pct);
			}
			ImGui::EndTable();
		}
	}

	ImGui::End();
}