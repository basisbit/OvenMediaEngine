//==============================================================================
//
//  OvenMediaEngine
//
//  Created by Hyunjun Jang
//  Copyright (c) 2019 AirenSoft. All rights reserved.
//
//==============================================================================
#pragma once

#include "cmaf_publisher.h"
#include "dash_publisher.h"
#include "hls_publisher.h"
#include "rtmp_publisher.h"
#include "webrtc_publisher.h"

namespace cfg
{
	struct Publishers : public Item
	{
		std::vector<const Publisher *> GetPublisherList() const
		{
			return {
				&_rtmp_publisher,
				&_hls_publisher,
				&_dash_publisher,
				&_cmaf_publisher,
				&_webrtc_publisher};
		}

		CFG_DECLARE_GETTER_OF(GetThreadCount, _thread_count)
		CFG_DECLARE_REF_GETTER_OF(GetRtmpPublisher, _rtmp_publisher)
		CFG_DECLARE_REF_GETTER_OF(GetHlsPublisher, _hls_publisher)
		CFG_DECLARE_REF_GETTER_OF(GetDashPublisher, _dash_publisher)
		CFG_DECLARE_REF_GETTER_OF(GetCmafPublisher, _cmaf_publisher)
		CFG_DECLARE_REF_GETTER_OF(GetWebrtcPublisher, _webrtc_publisher)

	protected:
		void MakeParseList() override
		{
			RegisterValue<Optional>("ThreadCount", &_thread_count);

			RegisterValue<Optional>("RTMP", &_rtmp_publisher);
			RegisterValue<Optional>("HLS", &_hls_publisher);
			RegisterValue<Optional>("DASH", &_dash_publisher);
			RegisterValue<Optional>("CMAF", &_cmaf_publisher);
			RegisterValue<Optional>("WebRTC", &_webrtc_publisher);
		}

		int _thread_count = 4;

		RtmpPublisher _rtmp_publisher;
		HlsPublisher _hls_publisher;
		DashPublisher _dash_publisher;
		CmafPublisher _cmaf_publisher;
		WebrtcPublisher _webrtc_publisher;
	};
}  // namespace cfg