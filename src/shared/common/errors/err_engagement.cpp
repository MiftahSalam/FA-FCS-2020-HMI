#include "err_engagement.h"

ErrEngagementTrackNotFound::ErrEngagementTrackNotFound() : BaseError(ERROR_CODE_ENGAGEMENT_TRACK_NOT_FOUND.first, ERROR_CODE_ENGAGEMENT_TRACK_NOT_FOUND.second)
{

}

ErrEngagementTrackNotEngageable::ErrEngagementTrackNotEngageable() : BaseError(ERROR_CODE_ENGAGEMENT_TRACK_NOT_ENGAGEABLE.first, ERROR_CODE_ENGAGEMENT_TRACK_NOT_ENGAGEABLE.second)
{

}

ErrEngagementTrackAlreadyEngaged::ErrEngagementTrackAlreadyEngaged() : BaseError(ERROR_CODE_ENGAGEMENT_TRACK_ALREADY_ENGAGED.first, ERROR_CODE_ENGAGEMENT_TRACK_ALREADY_ENGAGED.second)
{

}
