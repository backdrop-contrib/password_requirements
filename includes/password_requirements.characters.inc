<?php
/**
 * @file
 *
 * Functions for character based requirements.
 */

/**
 * Check the length of the password.
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
function password_requirements_characters_length_check($password, $username, array $config) {
  // Get the config into variables for ease of reference.
  $config_length = $config['count'];
  // Define the check requirement text in a human readable statement including
  // relevant config as appropriate.
  $check_requirement = t('Passwords must contain at least @count characters.', array(
    '@count' => $config_length,
  ));
  // Complete the check, capturing intermediate steps as appropriate.
  $actual_length = backdrop_strlen($password);
  $result = $actual_length >= $config_length;
  // If the check fails, prepare an explanation using the results.
  if (!$result) {
    if ($actual_length == 0) {
      $check_failure_explanation = t('Your password has 0 characters.');
    }
    else {
      $check_failure_explanation = format_plural($actual_length,
        'Your password only has 1 character.',
        'Your password only has @count characters.'
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

/**
 * Check how many numbers are in the password.
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
function password_requirements_characters_numbers_check($password, $username, array $config) {
  // Get the config into variables for ease of reference.
  $config_count = $config['count'];
  // Define the check requirement text in a human readable statement including
  // relevant config as appropriate.
  $check_requirement = format_plural($config_count,
    'Passwords must contain at least 1 number.',
    'Passwords must contain at least @count numbers.'
  );
  // Complete the check, capturing intermediate steps as appropriate.
  $actual_count = preg_match_all('/[[:digit:]]/', $password, $matches);
  $result = $actual_count >= $config_count;
  // If the check fails, prepare an explanation using the results.
  if (!$result) {
    if ($actual_count == 0) {
      $check_failure_explanation = t('Your password has @count numbers.', array(
        '@count' => $actual_count,
      ));
    }
    else {
      $check_failure_explanation = format_plural($actual_count,
        'Your password only has 1 number.',
        'Your password only has @count numbers.'
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

/**
 * Check how many letters are in the password and if they contain both upper and
 * lower case, if applicable.
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
function password_requirements_characters_letters_check($password, $username, array $config) {
  // Get the config into variables for ease of reference.
  $config_count = $config['count'];
  $config_mixed_case = $config['mixed_case'];
  // Define the check requirement text in a human readable statement including
  // relevant config as appropriate.
  if ($config_mixed_case) {
    $check_requirement = t('Passwords must contain at least @count letters and a mix of upper and lower case.', array(
      '@count' => $config_count,
    ));
  }
  else {
    $check_requirement = format_plural($config_count,
      'Passwords must contain at least 1 letter.',
      'Passwords must contain at least @count letters.'
    );
  }

  // Complete the check, capturing intermediate steps as appropriate.
  $actual_count = preg_match_all('/[[:alpha:]]/', $password, $matches);
  $count_result = $actual_count >= $config_count;
  $result = $count_result;
  if ($config_mixed_case) {
    $upper_case = preg_match('/[A-Z]/', $password);
    $lower_case = preg_match('/[a-z]/', $password);
    $mixed_case_result = $upper_case && $lower_case;
    $result = $count_result && $mixed_case_result;
  }

  // If the check fails, prepare an explanation using the results.
  if (!$result) {
    if ($config_mixed_case) {
      if (!$count_result && !$mixed_case_result) {
        if ($actual_count == 0) {
          $check_failure_explanation = t('Your password has 0 letters and you do not have a mix of upper and lower case letters.');
        }
        else {
          $check_failure_explanation = format_plural($actual_count,
            'Your password only has 1 letter and you do not have a mix of upper and lower case letters.',
            'Your password only has @count letters and you do not have a mix of upper and lower case letters.'
          );
        }
      }
      elseif ($count_result && !$mixed_case_result) {
        $check_failure_explanation = t('Your password has @count letters but you do not have a mix of upper and lower case letters.', array(
          '@count' => $actual_count,
        ));
      }
      elseif (!$count_result && $mixed_case_result) {
        $check_failure_explanation = t('Your password only has @count letters, although you do have a mix of upper and lower case letters.', array(
          '@count' => $actual_count,
        ));
      }
    }
    else {
      if ($actual_count == 0) {
        $check_failure_explanation = t('Your password has 0 letters.');
      }
      else {
        $check_failure_explanation = format_plural($actual_count,
          'Your password only has 1 letter.',
          'Your password only has @count letters.'
        );
      }
    }
  }

  // Compile the output.
  return array(
    'result' => $result,
    'check_requirement' => $check_requirement,
    'check_failure_explanation' => $check_failure_explanation ?? '',
  );
}

/**
 * Check how many special characters are in the password.
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
function password_requirements_characters_special_characters_check($password, $username, array $config) {
  // Get the config into variables for ease of reference.
  $config_count = $config['count'];
  $config_character_set = $config['character_set'];
  // Define the check requirement text in a human readable statement including
  // relevant config as appropriate.
  $check_requirement = format_plural($config_count,
    'Passwords must contain at least 1 special character. Special characters include: @character_set',
    'Passwords must contain at least @count special characters. Special characters include: @character_set',
     array('@character_set' => $config_character_set)
  );
  // Complete the check, capturing intermediate steps as appropriate.
  $matches = array();
  $pattern = '/[' . preg_quote($config_character_set, '/') . ']/';
  $actual_count = preg_match_all($pattern, $password, $matches);
  $result = $actual_count >= $config_count;
  // If the check fails, prepare an explanation using the results.
  if (!$result) {
    if ($actual_count == 0) {
      $check_failure_explanation = t('Your password has 0 special characters.');
    }
    else {
      $check_failure_explanation = format_plural($actual_count,
        'Your password only has 1 special character.',
        'Your password only has @count special characters.'
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
