<?php
/**
 * @file
 *
 * Functions for strength based requirements.
 */

/**
 * Check password against equivalent of Backdrop Strength Check.
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
 *
 * @see _user_password_evaluate_strength()
 */
function password_requirements_strength_backdrop_strength_check($password, $username, array $config) {
  // Get the config into variables for ease of reference.
  $config_level = $config['level'];
  $config_level_number = constant($config_level);
  $config_levels = array(
    'USER_PASSWORD_STRENGTH_WEAK' => t('Weak'),
    'USER_PASSWORD_STRENGTH_FAIR' => t('Fair'),
    'USER_PASSWORD_STRENGTH_GOOD' => t('Good'),
    'USER_PASSWORD_STRENGTH_STRONG' => t('Strong'),
  );
  $config_level_name = $config_levels[$config_level];
  // Define the check requirement text in a human readable statement including
  // relevant config as appropriate.
  $check_requirement = t('Password strength must be @config_level_name or better. It cannot be the same as your username and must be sufficiently long or complex.', array(
    '@config_level_name' => $config_level_name,
  ));
  // Complete the check, capturing intermediate steps as appropriate.
  $strength = 0;

  $has_lowercase = preg_match('/[a-z]+/', $password);
  $has_uppercase = preg_match('/[A-Z]+/', $password);
  $has_numbers = preg_match('/[0-9]+/', $password);
  $has_punctuation = preg_match('/[^a-zA-Z0-9]+/', $password);

  // Calculate the number of unique character sets within a string.
  $cardinality = ($has_lowercase * 26) + ($has_uppercase * 26) + ($has_numbers * 10) + ($has_punctuation * 33);

  // Assign strength based on the level of entropy within the password, times
  // its length.
  $length = backdrop_strlen($password);
  $strength = (log($cardinality) / log(2)) * $length + 1;

  // Adjust the strength so that we hit our desired password length for each
  // threshold. As computers improve, the recommended minimum length increases.
  $strength = $strength * USER_PASSWORD_STRENGTH_MODIFIER;

  // Check if password contains username.
  $pattern = '/' . preg_quote($username, '/') . '/';
  $username_check = preg_match($pattern, $password);
  if ($username_check) {
    $strength = 5;
  }

  $result = $strength >= $config_level_number;

  // If the check fails, prepare an explanation using the results.
  if (!$result) {
    if ($strength >= USER_PASSWORD_STRENGTH_GOOD) {
      $result_descriptor = t('good');
    }
    elseif ($strength >= USER_PASSWORD_STRENGTH_FAIR) {
      $result_descriptor = t('fair');
    }
    else {
      $result_descriptor = t('weak');
    }
    $check_failure_explanation = t('Your password strength is @strength_descriptor.', array(
      '@strength_descriptor' => $result_descriptor,
    ));
  }
  // Compile the output.
  return array(
    'result' => $result,
    'check_requirement' => $check_requirement,
    'check_failure_explanation' => $check_failure_explanation ?? '',
  );
}
