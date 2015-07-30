#pragma once

#include "Definitions.hpp"
#include "Vector.hpp"
#include "QAngle.hpp"

namespace SDK {
	struct AudioState_t {
		AudioState_t() {
			m_Origin.Init();
			m_Angles.Init();
			m_bIsUnderwater = false;
		}

		Vector m_Origin;
		QAngle m_Angles;
		bool m_bIsUnderwater;
	};
	
}