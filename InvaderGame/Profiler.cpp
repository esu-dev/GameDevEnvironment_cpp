#include "Profiler.h"
#include "imgui.h"

std::vector<Profiler::FrameData*> Profiler::_history;
int Profiler::_selectedFrameIndex = -1;

Profiler::FrameData* Profiler::GetLastFrameData()
{
	return _history[_history.size() - 1];
}

void Profiler::AddFrameData(FrameData* data)
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
		maxTime = max(maxTime, frame->totalTime);
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

		for (const auto& pair : frame->categories)
		{
			float h = (pair.second->totalTime / maxTime) * graphHeight;
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
		const FrameData* selectedFrame = _history[_selectedFrameIndex];
		ImGui::Text("Selected Frame: %d | Total: %.2f ms (%.1f FPS)", _selectedFrameIndex, selectedFrame->totalTime, 1000.0f / selectedFrame->totalTime);
		
		static ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;

		if (ImGui::BeginTable("DetailedHierarchy", 3, flags))
		{
			ImGui::TableSetupColumn("Category", ImGuiTableColumnFlags_NoHide);
			ImGui::TableSetupColumn("Time (ms)", ImGuiTableColumnFlags_WidthFixed, 100.0f);
			ImGui::TableSetupColumn("Percentage", ImGuiTableColumnFlags_WidthFixed, 100.0f);
			ImGui::TableHeadersRow();

			float totalTime = selectedFrame->totalTime;
			std::function<void(const FrameData*, int)> showCategory = [&](const FrameData* frameData, int indent) -> void
				{
					for (const auto& categoryPair : frameData->categories)
					{
						const std::string& categoryName = categoryPair.first;
						const auto& categoryData = categoryPair.second;

						ImGui::TableNextRow();
						ImGui::TableNextColumn();

						bool hasSubcategories = !categoryData->categories.empty();
						bool open = false;
						if (hasSubcategories)
						{
							open = ImGui::TreeNodeEx(categoryName.c_str(), ImGuiTreeNodeFlags_SpanFullWidth);
						}
						else
						{
							ImGui::TreeNodeEx(categoryName.c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);
						}

						if (indent == 0)
						{
							ImGui::TableNextColumn();
							ImGui::Text("%.3f ms", categoryData->totalTime);
							ImGui::TableNextColumn();
							ImGui::Text("%.1f%%", (categoryData->totalTime / totalTime) * 100.0f);
						}
						else
						{
							ImGui::TableNextColumn();
							ImGui::Indent(indent * 10);
							ImGui::TextDisabled("%.3f ms", categoryData->totalTime);
							ImGui::Unindent(indent * 10);

							ImGui::TableNextColumn();
							ImGui::Indent(indent * 10);
							ImGui::TextDisabled("%.1f%%", (categoryData->totalTime / totalTime) * 100.0f);
							ImGui::Unindent(indent * 10);
						}

						if (hasSubcategories && open)
						{
							showCategory(categoryData, indent + 1);

							/*for (const auto& subPair : categoryData->categories)
							{

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
								ImGui::TextDisabled("%s", subPair.first.c_str());
								ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

								ImGui::TableNextColumn();
								ImGui::TextDisabled("%.3f ms", subPair.second->totalTime);
								ImGui::TableNextColumn();
								ImGui::TextDisabled("%.1f%%", (subPair.second->totalTime / totalTime) * 100.0f);
							}*/
							ImGui::TreePop();
						}
					}
				};

			showCategory(selectedFrame, 0);

			ImGui::EndTable();
		}
	}

	ImGui::End();
}
