<?php

/**
 * @file
 * User session handling functions.
 *
 * Let Symfony handle sessions.
 */

/**
 * Initializes the session handler, starting a session if needed.
 */
function drupal_session_initialize() {
  global $user;
  $user = drupal_anonymous_user();
  date_default_timezone_set(drupal_get_user_timezone());
}

/**
 * Starts a session forcefully, preserving already set session data.
 *
 * @ingroup php_wrappers
 */
function drupal_session_start() {
}

/**
 * Commits the current session, if necessary.
 *
 * If an anonymous user already have an empty session, destroy it.
 */
function drupal_session_commit() {
}

/**
 * Returns whether a session has been started.
 */
function drupal_session_started($set = NULL) {
  /** @var \Symfony\Component\HttpFoundation\Session\Session $session */
  $session = $GLOBALS['session'];
  static $session_started = FALSE;
  if (isset($set)) {
    $session_started = $set;
  }
  return $session_started && $session->isStarted();
}

/**
 * Called when an anonymous user becomes authenticated or vice-versa.
 *
 * @ingroup php_wrappers
 */
function drupal_session_regenerate() {
}

/**
 * Ends a specific user's session(s).
 *
 * @param $uid
 *   User ID.
 */
function drupal_session_destroy_uid($uid) {
  // Nothing to do if we are not allowed to change the session.
  if (!drupal_save_session()) {
    return;
  }

  db_delete('sessions')
    ->condition('uid', $uid)
    ->execute();
}

/**
 * Determines whether to save session data of the current request.
 *
 * This function allows the caller to temporarily disable writing of
 * session data, should the request end while performing potentially
 * dangerous operations, such as manipulating the global $user object.
 * See http://drupal.org/node/218104 for usage.
 *
 * @param $status
 *   Disables writing of session data when FALSE, (re-)enables
 *   writing when TRUE.
 *
 * @return
 *   FALSE if writing session data has been disabled. Otherwise, TRUE.
 */
function drupal_save_session($status = NULL) {
  // PHP session ID, session, and cookie handling happens in the global scope.
  // This value has to persist across calls to drupal_static_reset(), since a
  // potentially wrong or disallowed session would be written otherwise.
  static $save_session = TRUE;
  if (isset($status)) {
    $save_session = $status;
  }
  return $save_session;
}
