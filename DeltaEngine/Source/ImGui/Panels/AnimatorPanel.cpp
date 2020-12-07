#include "AnimatorPanel.h"
#include "ImGui/IconsFontAwesome5.h"

#include "DeltaEngine.h"
#include "imgui.h"
#include "Core/GlobalStruct.h"
#include "Assets/AssetManager.h"
#include "ImGui/ImGuiMath.h"
#include "Input/Keys.h"

#include <string>

namespace DeltaEngine
{
AnimatorPanel::AnimatorPanel( std::string str, Editor &e )
  : IPanel( str, e )
{
  m_enabled = false;
}

AnimatorPanel::~AnimatorPanel()
{
  m_enabled = false;
}

void AnimatorPanel::Render()
{
  if ( ImGui::Begin( "Animator Editor", &m_enabled ) )
  {
    struct Node
    {
      int nID;
      char nodeName[128];
      ImVec2 nodePos, nodeSize;
      int connections;
      bool selected;

      Node(
        int id,
        const char *name,
        const ImVec2 &pos ) :
        nID { id },
        nodeName {},
        nodePos { pos },
        connections { 0 },
        selected { false }
      {
        strcpy_s( nodeName, name );
      }

      ImVec2 GetSlotPos( ImVec2 disp ) const
      {
        return ImVec2( nodePos.x + nodeSize.x / 2, nodePos.y + nodeSize.y / 2 ) + disp * 7.5f;
      }
    };

    struct NodeLink
    {
      int input, output;

      NodeLink( int in, int out )
      {
        input = in;
        output = out;
      }
    };

    // static vars
    static std::unordered_map<std::string, bool> paramSelected;
    static std::vector<Node> nodes;
    static std::vector<NodeLink> links;
    static ImVec2 scrolling = ImVec2( 0.0f, 0.0f );
    static bool loaded = false;
    static int nodeSelected = -1;
    static int transitionNode = -1;
    static int hoveredNode = -1;
    static int selectedTransition = -1;

    std::string deleteParam = "";
    ImGuiIO &io = ImGui::GetIO();

    AnimationController *controller = GetEnv().pManager->Get<AnimationController>( m_editor.selectedFile );

    ImGui::BeginChild( "Parameters", ImVec2( 150, 0 ) );
    ImGui::Text( "Parameters" );
    ImGui::Separator();
    if ( ImGui::Button( "Add..." ) )
    {
      std::string str = "NewParam";
      int i = 0; bool hasSameName = false;
      auto checkSameName = [&str]( std::string &s )
      {
        return !strcmp( str.c_str(), s.c_str() );
      };
      do
      {
        if ( hasSameName )
        {
          hasSameName = false;
          str = "NewParam" + std::to_string( i++ );
        }
        for ( auto &[pName, v] : controller->startingParameters )
        {
          hasSameName = !strcmp( str.c_str(), pName.c_str() );
          if ( hasSameName )
            break;
        }
      } while ( hasSameName );
      controller->startingParameters.push_back( { str, AnimationController::Parameter() } );
    }
    ImGui::Separator();
    if ( !loaded && controller )
    {
      nodes.clear();
      for ( auto &[ParamName, Value] : controller->startingParameters )
      {
        paramSelected[ParamName] = false;
      }
      int i = 0;
      for ( auto &[key, pos] : controller->editorPositions )
      {
        Vector2 v = controller->EditionPositionAt( key );
        nodes.push_back( Node( i++, key.Key().c_str(), ImVec2( v.x, v.y ) ) );
      }

      // entry animation
      {
        int entry = 0;
        while ( entry < nodes.size() && strcmp( nodes[entry].nodeName, controller->entryAnimation.c_str() ) )
          ++entry;
        if ( entry >= nodes.size() )
          nodes.push_back( Node( i++, controller->entryAnimation.c_str(), ImVec2( 0, 0 ) ) );
        if ( entry < nodes.size() )
          links.push_back( NodeLink( 0, entry ) );
      }
      // the rest of the animations
      for ( auto &[StartState, EndState, Conditions] : controller->transitions )
      {
        int start = 0, end = 0;
        while ( start < nodes.size() && strcmp( StartState.c_str(), nodes[start].nodeName ) )
          ++start;
        if ( start >= nodes.size() )
          nodes.push_back( Node( i++, StartState.c_str(), ImVec2( 0, 0 ) ) );
        while ( end < nodes.size() && strcmp( EndState.c_str(), nodes[end].nodeName ) )
          ++end;
        if ( end >= nodes.size() )
          nodes.push_back( Node( i++, EndState.c_str(), ImVec2( 0, 0 ) ) );
        links.push_back( NodeLink( start, end ) );
      }
      loaded = true;
    }
    for ( auto &[ParamName, Value] : controller->startingParameters )
    {
      ImGui::Selectable( ParamName.c_str(), &paramSelected[ParamName] );
      if ( paramSelected[ParamName] )
      {
        ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_Leaf |
          ImGuiTreeNodeFlags_NoTreePushOnOpen;

        nodeFlags |= ImGuiTreeNodeFlags_Selected;

        ImGui::Checkbox( "Bool", &Value.boolValue );
        ImGui::DragFloat( "Float", &Value.floatValue, 0.1f, 0.f, 0.f, "%0.1f" );
        if ( ImGui::Button( "Delete..." ) )
        {
          paramSelected.erase( ParamName );
          deleteParam = ParamName.c_str();
        }
      }
    }

    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginGroup();

    const float NODE_SLOT_RADIUS = 4.0f;
    const ImVec2 NODE_WINDOW_PADDING( 8.0f, 8.0f );

    ImGui::Text( "Hold middle mouse button to scroll (%.2f,%.2f)", scrolling.x, scrolling.y );

    ImVec2 p = ImGui::GetCursorScreenPos();

    // node graph
    ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 1, 1 ) );
    ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 0, 0 ) );
    ImGui::PushStyleColor( ImGuiCol_ChildBg, IM_COL32( 60, 60, 70, 200 ) );
    ImGui::BeginChild( "scrolling_region", ImVec2( ImGui::GetWindowContentRegionWidth() - 600, 0 ), true,
                       ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoMove );
    ImGui::PopStyleVar(); // WindowPadding
    ImGui::PushItemWidth( 120.0f );

    const ImVec2 offset = ImGui::GetCursorScreenPos() + scrolling;
    ImDrawList *draw_list = ImGui::GetWindowDrawList();

    // grid
    {
      ImU32 GRID_COLOR = IM_COL32( 200, 200, 200, 40 );
      float GRID_SZ = 100.0f;
      ImVec2 win_pos = ImGui::GetCursorScreenPos();
      ImVec2 canvas_sz = ImGui::GetWindowSize();
      for ( float x = fmodf( scrolling.x, GRID_SZ ); x < canvas_sz.x; x += GRID_SZ )
        draw_list->AddLine( ImVec2( x, 0.0f ) + win_pos, ImVec2( x, canvas_sz.y ) + win_pos, GRID_COLOR );
      for ( float y = fmodf( scrolling.y, GRID_SZ ); y < canvas_sz.y; y += GRID_SZ )
        draw_list->AddLine( ImVec2( 0.0f, y ) + win_pos, ImVec2( canvas_sz.x, y ) + win_pos, GRID_COLOR );
    }

    // display links
    draw_list->ChannelsSplit( 2 );
    draw_list->ChannelsSetCurrent( 0 ); // Background

    ImGui::SetCursorScreenPos( p );
    if ( ImGui::InvisibleButton( "##Scroll Area NULL", ImVec2(
      ImGui::GetWindowContentRegionWidth() - 300,
      ImGui::GetWindowHeight() ), ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight ) )
      nodeSelected = -1;
    ImGui::SetItemAllowOverlap();

    for ( NodeLink &link : links )
    {
      Node *in = &nodes[link.input];
      Node *out = &nodes[link.output];
      ImVec2 v = out->nodePos - in->nodePos;
      float vMag = sqrtf( v.x * v.x + v.y * v.y );
      v /= vMag;
      ImVec2 n = ImVec2( v.y, -v.x );
      ImVec2 p1 = offset + in->GetSlotPos( n );
      ImVec2 p2 = offset + out->GetSlotPos( n );
      draw_list->AddLine( p1, p2, IM_COL32( 255, 255, 255, 255 ), 3.0f );
      ImVec2 midpt = ( p1 + p2 ) / 2;
      v = p2 - p1;
      vMag = sqrtf( v.x * v.x + v.y * v.y );
      v /= vMag;
      n = ImVec2( v.y, -v.x );
      draw_list->AddTriangleFilled(
        midpt + v * 7.f,
        midpt - v * 7.f + n * 7.f,
        midpt - v * 7.f - n * 7.f,
        IM_COL32( 255, 255, 255, 255 ) );
    }
    if ( transitionNode >= 0 && transitionNode < nodes.size() )
    {
      ImVec2 v = io.MousePos - nodes[transitionNode].nodePos;
      float vMag = sqrtf( v.x * v.x + v.y * v.y );
      v /= vMag;
      ImVec2 p1 = offset + nodes[transitionNode].GetSlotPos( ImVec2( v.y, -v.x ) );
      draw_list->AddLine( p1, io.MousePos, IM_COL32( 255, 255, 255, 255 ), 3.0f );

      if ( ImGui::IsMouseClicked( ImGuiMouseButton_Left ) )
      {
        if ( hoveredNode >= 1 && hoveredNode < nodes.size() )
        {
          // create a new transition, then save and load
          controller->AddNewTransition(
            std::string( nodes[transitionNode].nodeName ), std::string( nodes[hoveredNode].nodeName ) );

          controller->SaveToFile();
          controller->LoadFromFile();
          loaded = false;
        }
        transitionNode = -1;
      }

      if ( ImGui::IsMouseClicked( ImGuiMouseButton_Right ) )
        transitionNode = -1;
    }

    // display nodes
    for ( Node &node : nodes )
    {
      ImGui::PushID( node.nID );
      ImVec2 node_rect_min = offset + node.nodePos;

      // display node contents first
      draw_list->ChannelsSetCurrent( 1 ); // foreground
      bool old_any_active = ImGui::IsAnyItemActive();
      ImGui::SetCursorScreenPos( node_rect_min + NODE_WINDOW_PADDING );
      ImGui::BeginGroup(); // lock horizontal position
      ImGui::Text( "%s", node.nodeName );
      ImGui::EndGroup();

      // save the size of what we have emitted and whether any of the widgets are being used
      bool node_widgets_active = ( !old_any_active && ImGui::IsAnyItemActive() );
      node.nodeSize = ImGui::GetItemRectSize() + NODE_WINDOW_PADDING + NODE_WINDOW_PADDING;
      ImVec2 node_rect_max = node_rect_min + node.nodeSize;

      // display node box
      draw_list->ChannelsSetCurrent( 0 ); // Background
      ImGui::SetCursorScreenPos( node_rect_min );
      if ( ImGui::InvisibleButton( "node", node.nodeSize, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight ) )
      {
        selectedTransition = -1;
        nodeSelected = node.nID;
      }

      if ( ImGui::IsItemHovered() )
        hoveredNode = node.nID;

      bool node_moving_active = ImGui::IsItemActive();
      if ( node_widgets_active || node_moving_active )
        nodeSelected = node.nID;
      if ( node_moving_active && ImGui::IsMouseDragging( ImGuiMouseButton_Left ) )
        node.nodePos = node.nodePos + io.MouseDelta;

      ImU32 node_bg_color = nodeSelected == node.nID ? IM_COL32( 102, 102, 102, 255 ) : IM_COL32( 85, 85, 85, 255 );
      if ( node.nID == 0 )
        node_bg_color = IM_COL32( 0, 102, 0, 255 );
      if ( node.nID == 1 )
        node_bg_color = IM_COL32( 102, 0, 0, 255 );
      draw_list->AddRectFilled( node_rect_min, node_rect_max, node_bg_color, 4.0f );
      if ( nodeSelected == node.nID )
        draw_list->AddRect( node_rect_min, node_rect_max, IM_COL32( 0, 102, 153, 255 ), 4.0f, 15, 3.f );

      ImGui::PopID();
    }
    draw_list->ChannelsMerge();

    // open context menu
    if ( ImGui::IsMouseReleased( ImGuiMouseButton_Right ) )
      if ( ImGui::IsWindowHovered( ImGuiHoveredFlags_AllowWhenBlockedByPopup ) )
      {
        ImGui::OpenPopup( "Animator Node Context Menu" );
      }

    // draw context menu
    ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 8, 8 ) );
    if ( ImGui::BeginPopup( "Animator Node Context Menu" ) )
    {
      Node *node = ( nodeSelected >= 0 && nodeSelected < nodes.size() ) ? &nodes[nodeSelected] : NULL;
      if ( node )
      {
        ImGui::Text( "Clip: \"%s\"", node->nodeName );
        ImGui::Separator();
        if ( ImGui::MenuItem( "Make Transition" ) )
        {
          // no transition starts from entry or exit
          if ( node->nID > 1 && node->nID < nodes.size() )
            transitionNode = node->nID;
        }
        if ( ImGui::MenuItem( "Delete" ) )
        {
          int i = 0;
          for ( auto &[key, pos] : controller->editorPositions )
          {
            if ( key == node->nodeName )
              controller->editorPositions.erase( controller->editorPositions.begin() + i );
            ++i;
          }
          i = 0;
          for ( auto &[StartState, EndState, Conditions] : controller->transitions )
          {
            if ( !strcmp( StartState.c_str(), node->nodeName ) ||
                 !strcmp( EndState.c_str(), node->nodeName ) )
              controller->transitions.erase( controller->transitions.begin() + i );
            ++i;
          }
          nodes.erase( nodes.begin() + nodeSelected );
          nodeSelected = -1;
        }
      }
      ImGui::EndPopup();
    }
    ImGui::PopStyleVar();

    if ( !deleteParam.empty() )
    {
      for ( auto it = controller->startingParameters.begin();
            it != controller->startingParameters.end(); ++it )
        if ( !strcmp( ( *it ).first.c_str(), deleteParam.c_str() ) )
        {
          controller->startingParameters.erase( it );
          break;
        }
      for ( auto &[startState, endState, cons] : controller->transitions )
      {
        for ( auto it = cons.begin();
              it != cons.end();)
        {
          auto &[paramName, conType, conVal] = *it;
          if ( !strcmp( paramName.c_str(), deleteParam.c_str() ) )
            it = cons.erase( it );
          else
            ++it;
        }
      }
    }

    if ( ImGui::IsMouseReleased( ImGuiMouseButton_Left ) )
    {
      controller->editorPositions.clear();
      for ( Node &node : nodes )
      {
        controller->editorPositions.push_back( { node.nodeName, Vector2( node.nodePos.x, node.nodePos.y ) } );
      }
      controller->SaveToFile();
      controller->LoadFromFile();
    }

    // scrolling
    if ( ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && ImGui::IsMouseDragging( ImGuiMouseButton_Middle, 0.0f ) )
      scrolling = scrolling + io.MouseDelta;

    ImGui::PopItemWidth();
    ImGui::EndChild();
    if ( ImGui::BeginDragDropTarget() )
    {
      const ImGuiPayload *assetpayload = ImGui::AcceptDragDropPayload( "ASSETFILES" );
      if ( assetpayload )
      {
        std::string assetpayload_n = *static_cast<std::string *>( assetpayload->Data );
        std::filesystem::path file { assetpayload_n };
        if ( file.extension() == ".clip" )
          controller->editorPositions.push_back(
          std::pair( AssetKey { assetpayload_n.substr( 0, assetpayload_n.find_last_of( '.' ) ) },
          Vector2 { io.MousePos.x - offset.x, io.MousePos.y - offset.y } ) );
        if ( controller->entryAnimation.empty() )
          controller->entryAnimation = assetpayload_n.substr( 0, assetpayload_n.find_last_of( '.' ) );
        loaded = false;
      }
    }

    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
    ImGui::EndGroup();

    // inspector
    {
      ImGui::SameLine();
      if ( ImGui::BeginChild( "Transition Inspector" ) )
      {
        ImGui::Text( "Inspector" );
        ImGui::Separator();

        if ( nodeSelected >= 0 && nodeSelected < nodes.size() )
        {
          if ( nodeSelected == 0 )
          {
            ImGui::Text( "Entry state has no editable properties" );
          }
          else if ( nodeSelected == 1 )
          {
            ImGui::Text( "Exit state has no editable properties" );
          }
          else
          {
            ImGui::Text( "%s", nodes[nodeSelected].nodeName );
            //
            ImGui::Text( "%s", nodes[nodeSelected].nodeName );

            std::vector<AssetKey> clip_vec;

            for ( auto &[key, data] : GetEnv().pManager->List<AnimationClip>() )
              if ( data )
                clip_vec.push_back( key );

            size_t selection = 0;
            for ( size_t i = 0; i < clip_vec.size(); i++ )
              if ( nodes[nodeSelected].nodeName == clip_vec[i].Key() )
                selection = i;

            size_t initial = selection;
            const char *clip_key = clip_vec[selection].Key().c_str();

            if ( ImGui::BeginCombo( "Clip Selection", clip_key ) )
            {
              for ( size_t i = 0; i < clip_vec.size(); i++ )
              {
                const bool is_selected = ( selection == i );
                if ( ImGui::Selectable( clip_vec[i].Key().c_str(), is_selected ) )
                  selection = i;

                if ( is_selected )
                  ImGui::SetItemDefaultFocus();
              }
              ImGui::EndCombo();
            }
            ImGui::Separator();
            ImGui::Text( "Transitions" );
            ImGui::Separator();
            int i = 0;
            for ( auto &[startState, endState, cons] : controller->transitions )
            {
              if ( !strcmp( startState.c_str(), nodes[nodeSelected].nodeName ) )
              {
                std::string str = startState + "->" + endState;
                if ( ImGui::Selectable( str.c_str(), selectedTransition == i ) )
                  selectedTransition = i;
              }
              ++i;
            }
            //
            ImGui::Separator();
            ImGui::Text( "Conditions" );
            ImGui::Separator();
            if ( selectedTransition >= 0 && selectedTransition < controller->transitions.size() )
            {
              std::vector<const char *> allParameters;
              for ( auto &param : controller->startingParameters )
                allParameters.push_back( param.first.c_str() );
              for ( auto &[paramName, conType, conVal] : std::get<2>( controller->transitions[selectedTransition] ) )
              {
                static const char *conditions[] { "Bool Equal", "Equal", "Not Equal", "Greater", "Less" };
                if ( ImGui::BeginCombo( "Parameter", paramName.c_str() ) )
                {
                  for ( auto &param : allParameters )
                  {
                    bool isSelected = !strcmp( paramName.c_str(), param );
                    if ( ImGui::Selectable( param, isSelected ) )
                    {

                    }

                    if ( isSelected )
                      ImGui::SetItemDefaultFocus();
                  }
                  ImGui::EndCombo();
                }

                ImGui::Combo( "Condition", (int *) &conType, conditions, IM_ARRAYSIZE( conditions ) );
                ImGui::DragFloat( "Value", &conVal );
                ImGui::Separator();
              }
              if ( allParameters.size() )
              {
                if ( ImGui::Button( "Add Condition" ) )
                {
                  std::get<2>( controller->transitions[selectedTransition] ).push_back( {
                    std::string( allParameters[0] ), AnimationController::Conditions::BoolEqual, 0.f }
                  );
                }
              }
              else
              {
                ImGui::Text( "There are no parameters!" );
                ImGui::Text( "Add parameters to add conditions" );
              }
            }
            // Update Combo Selection
            if ( initial != selection )
            {
              // Update Editor
              auto it = std::find_if( controller->editorPositions.begin(),
                                      controller->editorPositions.end(),
                                      [&]( const std::pair<AssetKey, Vector2> pair )
              {
                return pair.first == clip_vec[initial];
              } );
              if ( it != controller->editorPositions.end() )
                it->first = clip_vec[selection];


              // Update Transitions
              std::for_each( controller->transitions.begin(), controller->transitions.end(), [&]( AnimationController::Transition &t )
              {
                auto &[first, second, third] = t;
                if ( first == clip_vec[initial].Key() )
                  first = clip_vec[selection].Key();
                else if ( second == clip_vec[initial].Key() )
                  second = clip_vec[selection].Key();
              } );

              loaded = false;
            }
          }
        }
        else
        {
          ImGui::Text( "Nothing selected" );
          selectedTransition = -1;
        }
      }
      ImGui::EndChild();
    }
  }
  ImGui::End();
}
}
