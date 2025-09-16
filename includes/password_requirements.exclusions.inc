<?php
/**
 * @file
 *
 * Functions for exclusion based requirements.
 */

/**
 * Check that the password does not contain the username.
 *
 * @param string $password
 *   The password to be checked.
 * @param string $username
 *   The username to check/ignore.
 * @param array $config
 *   The config for the check.
 *
 * @return array
 *   An array containing:
 *     - result - boolean - pass (TRUE) or fail (FALSE)
 *     - check_requirement - string - the requirement with the number in a
 *       human readable statement.
 *     - check_failure_explanation - string - An optional string, if check has
 *       failed, to explain why it has failed.
 */
function password_requirements_exclusions_username_check($password, $username, array $config) {
  // Define the check requirement text in a human readable statement including
  // relevant config as appropriate.
  $check_requirement = t('Passwords must not contain username.');
  // Complete the check, capturing intermediate steps as appropriate.
  $username_lowercase = backdrop_strtolower($username);
  $password_lowercase = backdrop_strtolower($password);
  $result = strpos($password_lowercase, $username_lowercase) === FALSE;

  // If the check fails, prepare an explanation using the results.
  if (!$result) {
    $check_failure_explanation = t('Your password contains your username.');
  }
  // Compile the output.
  return array(
    'result' => $result,
    'check_requirement' => $check_requirement,
    'check_failure_explanation' => $check_failure_explanation ?? '',
  );
}

/**
 * Check for previous passwords
 *
 * @param string $password
 *   The password to be checked.
 * @param string $username
 *   The username to check/ignore.
 * @param array $config
 *   The config for the check.
 *
 * @return array
 *   An array containing:
 *     - result - boolean - pass (TRUE) or fail (FALSE)
 *     - check_requirement - string - the requirement with the number in a
 *       human readable statement.
 *     - check_failure_explanation - string - An optional string, if check has
 *       failed, to explain why it has failed.
 */
function password_requirements_exclusions_past_passwords_check($password, $username, array $config) {
  // Get the config into variables for ease of reference.
  $config_count = $config['count'];
  // Define the check requirement text in a human readable statement including
  // relevant config as appropriate.
  $check_requirement = format_plural($config_count,
    'Password must not match your previous password.',
    'Password must not match any of your past @count passwords'
  );
  // Complete the check, capturing intermediate steps as appropriate.
  // Load the user using the username.
  $user = user_load_by_name($username);
  if (!$user) {
    // If user cannot be loaded set an error message and prepare the result and
    // explanation.
    backdrop_set_message(t("The selected user (@username) does not exist or cannot be loaded.", array(
      '@username' => $username,
    )), 'error');
    $result = FALSE;
    $check_failure_explanation = t('The user could not be loaded and therefore the password history could not be checked.');
  }
  else {
    // Get the user ID.
    $user_id = $user->uid;
    // Get the password history for the user limited to the number of passwords
    // to check.
    $query = db_select('password_requirements_password_history', 'h')
      ->condition('h.uid', $user_id)
      ->fields('h', array('hid', 'uid', 'pass', 'created'))
      ->orderBy('created', 'DESC')
      ->range(0, $config_count);
    $password_history = $query->execute();
    // Start with a pass result and make fail if there is history AND the
    // password matches.
    $result = TRUE;
    // If the password history is not empty, loop through the retrieved password
    // history and check the password against them.
    if (!empty($password_history)) {
      // Password file needed to check the password.
      include_once BACKDROP_ROOT . '/' . settings_get('password_inc', 'core/includes/password.inc');
      // Create a temporary clone of the $user object.
      $temporary_user = clone $user;
      // Check each password in the history against the entered password.
      foreach ($password_history as $record) {
        $old_password = $record->pass;
        $temporary_user->pass = $old_password;
        if (user_check_password($password, $temporary_user)) {
          $result = FALSE;
          break;
        }
      }
    }
    // If the check fails, prepare an explanation using the results.
    if (!$result) {
      $check_failure_explanation = format_plural($config_count,
        'Your password matches your previous password.',
        'Your password matches one of your past @count passwords.'
      );
    }
  }

  // Compile the output.
  return array(
    'result' => $result,
    'check_requirement' => $check_requirement,
    'check_failure_explanation' => $check_failure_explanation ?? '',
  );
}
