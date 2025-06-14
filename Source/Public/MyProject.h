// MyProject.h
#pragma once

#include "CoreMinimal.h"

/**
 * @file MyProject.h
 * @brief General project-level declarations: log category and a helper accessor.
 */

/**
 * @brief Global logging category for the MyProject module.
 */
DECLARE_LOG_CATEGORY_EXTERN(LogMyProject, Log, All);

/**
 * @brief Returns a reference to the project's log category.
 * 
 * This inline function is convenient for use in templates and logging macros.
 * @return Reference to the log category.
 */
inline FLogCategoryBase& MyProjectLog() { return LogMyProject; }
