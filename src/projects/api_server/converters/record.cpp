//==============================================================================
//
//  OvenMediaEngine
//
//  Created by Hyunjun Jang
//  Copyright (c) 2020 AirenSoft. All rights reserved.
//
//==============================================================================
#include "record.h"

#include "common.h"

namespace api
{
	namespace conv
	{
		static void SetRecordStreamTracks(Json::Value &parent_object, const char *key, const std::map<int32_t, std::shared_ptr<MediaTrack>> &tracks, Optional optional)
		{
			CONVERTER_RETURN_IF(false, Json::arrayValue);

			for (auto &item : tracks)
			{
				auto &track = item.second;

				object.append(track->GetId());
			}
		}

		static void SetRecordStream(Json::Value &parent_object, const char *key, const info::Stream &stream, Optional optional)
		{
			CONVERTER_RETURN_IF(false, Json::objectValue);

			SetString(object, "name", stream.GetName(), Optional::False);
			SetRecordStreamTracks(object, "tracks", stream.GetTracks(), Optional::False);
		}

		// Example of Record Start
		// ----------------------
		// 	{
		// 		"id": "<CustomId | AutoGeneratedId>",
		// 		"stream" : {
		// 			"name" : "<OutputStreamName>",
		// 			"tracks" : [ 101, 102 ]
		// 		}
		// 		"interval": <Split recording time(ms)>,
		// 		"schedule": "* *" # pattern of crontab style. only use minutes and hours
		// 	}
		// 	* The interval and schedule parameters are not available at the same time.
		//
		//
		// Example of Record Stop
		// ----------------------
		// 	{
		// 		"id": "<CustomId | AutoGeneratedId>"
		// 	}
		std::shared_ptr<info::Record> RecordFromJson(const Json::Value &json_body)
		{
			auto record = std::make_shared<info::Record>();

			// <Required>
			auto json_id = json_body["id"];
			if (json_id.empty() == false && json_id.isString() == true)
			{
				record->SetId(json_id.asString().c_str());
			}

			// <Optional>
			auto json_stream = json_body["stream"];
			if (json_stream.empty() == false || json_stream.isObject() == true)
			{
				info::Stream stream(StreamSourceType::Transcoder);

				// <Required>
				auto json_stream_name = json_stream["name"];
				if (json_stream_name.empty())
				{
					json_stream_name = "";
				}

				stream.SetName(json_stream_name.asString().c_str());

				// <Optional>
				auto json_stream_tracks = json_stream["tracks"];
				if (json_stream_tracks.empty() == false && json_stream_tracks.isArray() == true)
				{
					for (uint32_t i = 0; i < json_stream_tracks.size(); i++)
					{
						if (json_stream_tracks[i].isInt())
						{
							auto media_track = std::make_shared<MediaTrack>();
							media_track->SetId(json_stream_tracks[i].asInt());

							stream.AddTrack(media_track);
						}
					}
				}

				record->SetStream(stream);
			}
			else
			{
				info::Stream stream(StreamSourceType::Transcoder);
				record->SetStream(stream);
			}

			// <Optional>
			auto json_file_path = json_body["filePath"];
			if (json_file_path.empty() == false && json_file_path.isString() == true)
			{
				record->SetFilePath(json_file_path.asString().c_str());
			}

			// <Optional>
			auto json_info_path = json_body["infoPath"];
			if (json_info_path.empty() == false && json_info_path.isString() == true)
			{
				record->SetInfoPath(json_info_path.asString().c_str());
			}

			// <Optional>
			auto json_interval = json_body["interval"];
			if (json_interval.empty() == false && json_interval.isInt() == true)
			{
				record->SetInterval(json_interval.asInt());
			}
			else
			{
				record->SetInterval(0);
			}

			// <Optional>
			auto json_schedule = json_body["schedule"];
			if (json_schedule.empty() == false && json_schedule.isString() == true)
			{
				record->SetSchedule(json_schedule.asString().c_str());
			}
			else
			{
				record->SetSchedule("");
			}

			// <Optional>
			auto json_metadata = json_body["metadata"];
			if (json_metadata.empty() == false && json_metadata.isString() == true)
			{
				record->SetMetadata(json_metadata.asString().c_str());
			}

			return record;
		}

		Json::Value JsonFromRecord(const std::shared_ptr<info::Record> &record)
		{
			Json::Value response(Json::ValueType::objectValue);

			SetString(response, "id", record->GetId(), Optional::False);

			SetString(response, "metadata", record->GetMetadata(), Optional::True);

			SetString(response, "vhost", record->GetVhost(), Optional::False);

			SetString(response, "app", record->GetApplication(), Optional::False);

			SetRecordStream(response, "stream", record->GetStream(), Optional::False);

			SetString(response, "state", record->GetStateString(), Optional::False);

			SetString(response, "filePath", record->GetFilePath(), Optional::False);

			SetString(response, "infoPath", record->GetInfoPath(), Optional::False);

			if (record->GetInterval() > 0)
			{
				SetInt(response, "interval", record->GetInterval());
			}

			if (record->GetSchedule().IsEmpty() == false)
			{
				SetString(response, "schedule", record->GetSchedule(), Optional::True);
			}

			SetInt64(response, "recordBytes", record->GetRecordBytes());

			SetInt64(response, "recordTime", record->GetRecordTime());

			SetInt64(response, "totalRecordBytes", record->GetRecordTotalBytes());

			SetInt64(response, "totalRecordTime", record->GetRecordTotalTime());

			SetInt(response, "sequence", record->GetSequence());

			SetTimestamp(response, "createdTime", record->GetCreatedTime());

			if (record->GetRecordStartTime() != std::chrono::system_clock::from_time_t(0))
			{
				SetTimestamp(response, "startTime", record->GetRecordStartTime());
			}
			if (record->GetRecordStopTime() != std::chrono::system_clock::from_time_t(0))

			{
				SetTimestamp(response, "finishTime", record->GetRecordStopTime());
			}

			return response;
		}

	}  // namespace conv
}  // namespace api