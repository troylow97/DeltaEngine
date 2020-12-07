#include "ImGui/Panels/AudioPanel.h"

#include "Audio/AudioEngine.h"
#include "Core/Utils/FileDialog.h"
#include "Core/Utils/Json/JsonFile.h"
#include "Core/Utils/FileUtils.h"
#include "ImGui/IconsFontAwesome5.h"

namespace DeltaEngine
{
AudioPanel::AudioPanel( std::string str, Editor &e ) :
  IPanel( str, e )
{
  m_enabled = true;
  if ( FileUtils::FileExists( "Audio/Settings.cfg" ) )
  {
    JsonFile file;
    config.banks_config.clear();
    config.sounds_config.clear();
    file.StartReader( "Audio/Settings.cfg" ).LoadObject( config ).EndReader();
  }
}

AudioPanel::~AudioPanel()
{
  m_enabled = false;
}

void AudioPanel::Render()
{
  if ( ImGui::Begin( m_name.c_str(), &m_enabled,
       ImGuiWindowFlags_MenuBar ) )
  {
    if ( ImGui::BeginMenuBar() )
    {
      if ( ImGui::Button( " Open " ) )
      {
        std::optional<std::string> path = FileDialogs::OpenFile( "AudioConfig (*.cfg)\0*.cfg\0" );

        if ( path )
        {
          JsonFile file;
          config.banks_config.clear();
          config.sounds_config.clear();
          file.StartReader( *path ).LoadObject( config ).EndReader();
          for ( const auto &ref : config.banks_config )
            if ( !AudioEngine::IsLoadedBank( ref.path ) )
              AudioEngine::LoadBank( ref.path, AUDIOENGINE_LOAD_BANK_NORMAL );
        }
      }

      if ( ImGui::Button( " Save " ) )
      {
        FileUtils::CreateFile( "Audio/Settings.cfg" );
        JsonFile file;
        AudioConfig c;
        c.banks_config = config.banks_config;
        c.sounds_config = config.sounds_config;
        file.StartWriter( "Audio/Settings.cfg" ).StartObject().WriteObject( c ).EndObject().EndWriter();
      }
      ImGui::EndMenuBar();
    }

    ImVec2 size = ImGui::GetContentRegionAvail();

    static size_t c_id { 0 };
    static std::string c_sound {};
    static std::string c_selection {};
    if ( ImGui::BeginChild( "Sound Configuration", { size.x, size.y / 2.0f } ) )
    {
      ImGui::Text( "Audio Files" );
      ImGui::Separator();
      for ( auto &ref : config.sounds_config )
      {
        if ( c_selection != ref.path )
          ImGui::GetStateStorage()->SetInt( ImGui::GetID( ref.path.c_str() ), 0 );

        ImGui::BeginGroup();
        if ( ImGui::CollapsingHeader( ref.path.c_str(), ImGuiTreeNodeFlags_AllowItemOverlap ) )
        {
          c_selection = ref.path;
          ImGui::Dummy( ImVec2( 0.0f, 10.0f ) );

          if ( FileUtils::FileExists( ref.path ) )
          {
            const float original = Editor::font_awesome->Scale;
            Editor::font_awesome->Scale = 0.5f;
            ImGui::PushFont( Editor::font_awesome );
            if ( ImGui::Button( ( ICON_FA_PLAY_CIRCLE "##" + ref.path ).c_str(), { 26, 26 } ) )
            {
              if ( AudioEngine::IsChannelPlaying( c_id ) )
                AudioEngine::StopChannel( c_id );
              c_id = AudioEngine::Play( ref.path );
              c_sound = ref.path;
            }
            ImGui::SameLine();
            if ( ImGui::Button( ( ICON_FA_STOP_CIRCLE "##" + ref.path ).c_str(), { 26, 26 } ) )
            {
              AudioEngine::StopChannel( c_id );
              c_sound.clear();
            }
            ImGui::PopFont();
            Editor::font_awesome->Scale = original;
            ImGui::SameLine();
            unsigned e_pos = AudioEngine::GetSoundLength( ref.path );
            unsigned e_pos_min = e_pos / 60000;
            unsigned e_pos_sec = ( e_pos % 60000 ) / 1000;
            auto t_size = ImGui::CalcTextSize( "00:00 / 00:00" );

            if ( AudioEngine::IsChannelPlaying( c_id ) && c_sound == ref.path )
            {
              unsigned c_pos = AudioEngine::GetChannelPlaybackPosition( c_id );
              unsigned c_pos_min = c_pos / 60000;
              unsigned c_pos_sec = ( c_pos % 60000 ) / 1000;
              float frac = (float) c_pos / (float) e_pos;
              ImGui::ProgressBar( frac, { ImGui::GetContentRegionAvailWidth() - t_size.x,0 } );
              ImGui::SameLine();
              ImGui::Text( "%d:%02d / %d:%02d", c_pos_min, c_pos_sec, e_pos_min, e_pos_sec );
            }
            else
            {
              ImGui::ProgressBar( 0, { ImGui::GetContentRegionAvailWidth() - t_size.x,0 } );
              ImGui::SameLine();
              ImGui::Text( "%d:%02d / %d:%02d", 0, 0, e_pos_min, e_pos_sec );
            }
            ImGui::Dummy( ImVec2( 0.0f, 10.0f ) );
            ImGui::Checkbox( ( "Loop##" + ref.path ).c_str(), &ref.loop );
            ImGui::SameLine();
            ImGui::Checkbox( ( "Stream##" + ref.path ).c_str(), &ref.stream );
            ImGui::SameLine();
            ImGui::Checkbox( ( "3D##" + ref.path ).c_str(), &ref.is3D );

          }
          else
            ImGui::Text( "FILE - %s CANNOT BE FOUND", ref.path.c_str() );
          ImGui::Dummy( ImVec2( 0.0f, 20.0f ) );
        }
        ImGui::EndGroup();

        ImGui::SameLine();
        ImGui::SetCursorPosX( ImGui::GetWindowContentRegionWidth() - 30.0f );
        const float original = Editor::font_awesome->Scale;
        Editor::font_awesome->Scale = 0.5f;
        ImGui::PushFont( Editor::font_awesome );
        ImGui::PushID( ( "Close" + ref.path ).c_str() );
        if ( ImGui::Button( ICON_FA_TIMES, { 30.0f,26.0f } ) )
        {
          config.sounds_config.erase( std::find_if( config.sounds_config.begin(),
                                      config.sounds_config.end(),
                                      [&]( const SoundConfig &lhs )
          {
            return lhs.path == ref.path;
          } ) );
          ImGui::PopID();
          ImGui::PopFont();
          Editor::font_awesome->Scale = original;
          break;
        }
        ImGui::PopID();
        ImGui::PopFont();
        Editor::font_awesome->Scale = original;


        if ( ImGui::BeginDragDropSource( ImGuiDragDropFlags_SourceNoDisableHover |
             ImGuiDragDropFlags_SourceAllowNullID ) )
        {
          ImGui::SetDragDropPayload( "Sound File", &c_selection, sizeof( std::string ) );
          ImGui::Text( c_selection.c_str() );
          ImGui::EndDragDropSource();
        }
      }
    }
    ImGui::EndChild();
    if ( ImGui::BeginDragDropTarget() )
    {
      if ( const ImGuiPayload *payload = ImGui::AcceptDragDropPayload( "ASSETFILES" ); payload )
      {
        std::string file = *static_cast<std::string *>( payload->Data );
        std::filesystem::path p { file };
        if ( p.extension() == ".wav" ||
             p.extension() == ".ogg" ||
             p.extension() == ".mp3" )
          config.sounds_config.push_back( { file } );
      }
      ImGui::EndDragDropTarget();
    }


    static size_t e_id { 0 };
    static std::string e_bank_selection;
    static std::string e_selection;
    if ( ImGui::BeginChild( "Bank Configuration" ) )
    {
      ImGui::Text( "Bank Files" );
      ImGui::Separator();
      for ( auto &ref : config.banks_config )
      {
        if ( e_bank_selection != ref.path )
          ImGui::GetStateStorage()->SetInt( ImGui::GetID( ref.path.c_str() ), 0 );
        ImGui::BeginGroup();
        if ( ImGui::CollapsingHeader( ref.path.c_str(), ImGuiTreeNodeFlags_AllowItemOverlap  ) )
        {
          e_bank_selection = ref.path;
          if ( FileUtils::FileExists( ref.path ) )
          {
            if ( !AudioEngine::IsLoadedBank( ref.path ) )
              AudioEngine::LoadBank( ref.path, AUDIOENGINE_LOAD_BANK_NORMAL );
            for ( auto a_event : AudioEngine::EventList( ref.path ) )
            {
              if ( e_selection != a_event )
                ImGui::GetStateStorage()->SetInt( ImGui::GetID( a_event.c_str() ), 0 );
              if ( ImGui::TreeNodeEx( a_event.c_str() ) )
              {
                e_selection = a_event;

                if ( ImGui::BeginDragDropSource( ImGuiDragDropFlags_SourceNoDisableHover |
                     ImGuiDragDropFlags_SourceAllowNullID ) )
                {
                  ImGui::SetDragDropPayload( "Sound Event", &e_selection, sizeof( std::string ) );
                  ImGui::Text( e_selection.c_str() );
                  ImGui::EndDragDropSource();
                }

                const float original = Editor::font_awesome->Scale;
                Editor::font_awesome->Scale = 0.5f;
                ImGui::PushFont( Editor::font_awesome );
                if ( ImGui::Button( ( ICON_FA_PLAY_CIRCLE "##" + ref.path ).c_str(), { 26, 26 } ) )
                {
                  if ( AudioEngine::IsEventPlaying( e_id ) )
                    AudioEngine::StopEvent( e_id );
                  e_id = AudioEngine::Play2DEvent( a_event );
                }
                ImGui::SameLine();
                if ( ImGui::Button( ( ICON_FA_STOP_CIRCLE "##" + ref.path ).c_str(), { 26, 26 } ) )
                  AudioEngine::StopEvent( e_id );
                ImGui::PopFont();
                Editor::font_awesome->Scale = original;
                ImGui::TreePop();
              }

            }
          }
          else
            ImGui::Text( "FILE - %s CANNOT BE FOUND", ref.path.c_str() );
        }
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::SetCursorPosX( ImGui::GetWindowContentRegionWidth() - 30.0f );
        const float original = Editor::font_awesome->Scale;
        Editor::font_awesome->Scale = 0.5f;
        ImGui::PushFont( Editor::font_awesome );
        ImGui::PushID( ( "Close" + ref.path ).c_str() );
        if ( ImGui::Button( ICON_FA_TIMES, { 30.0f,26.0f } ) )
        {
          config.banks_config.erase( std::find_if( config.banks_config.begin(),
                                      config.banks_config.end(),
                                      [&]( const BankConfig &lhs )
          {
            return lhs.path == ref.path;
          } ) );
          ImGui::PopID();
          ImGui::PopFont();
          Editor::font_awesome->Scale = original;
          break;
        }
        ImGui::PopID();
        ImGui::PopFont();
        Editor::font_awesome->Scale = original;
      }
    }
    ImGui::EndChild();

    if ( ImGui::BeginDragDropTarget() )
    {
      if ( const ImGuiPayload *payload = ImGui::AcceptDragDropPayload( "ASSETFILES" ); payload )
      {
        std::string file = *static_cast<std::string *>( payload->Data );
        std::filesystem::path p { file };
        if ( p.extension() == ".bank" )
          config.banks_config.push_back( { file } );

        if ( !AudioEngine::IsLoadedBank( file ) )
          AudioEngine::LoadBank( file, AUDIOENGINE_LOAD_BANK_NORMAL );
      }
      ImGui::EndDragDropTarget();
    }
  }
  ImGui::End();
}
}
