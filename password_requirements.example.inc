<?php
/**
 * @file
 *
 * An example for adding a new group with a check, and an example function to
 * use as a base for a new check in an existing function.
 *
 * If you add a new group, you will also need to add the group machine name into
 * the $requirement_groups array of password_requirements_active_checks() in
 * password_requirements.module:
 *
 * @code
 * // Define the requirement groups. Add to this array if additional groups are
 * // added.
 * $requirement_groups = array(
 *   'characters',
 *   'exclusions',
 *   'strength',
 *   'GROUP',
 * );
 * @endcode
 *
 * If you add a new check to an existing group then there is less to do.
 *
 * You will also need to at least add an active setting to
 * password_requirements.admin.inc within password_requirements_settings().
 *
 * Here is an example for a group and check that only has an active setting.
 * Look at other checks for examples of other configuration and config
 * validation.
 * @code
 *   // GROUP name.
 * $form['requirements']['GROUP'] = array(
 *   '#type' => 'fieldset',
 *   '#tree' => TRUE,
 *   '#title' => t('GROUP name'),
 *   '#description' => t('GROUP description.'),
 *   '#collapsible' => TRUE,
 * );
 * // CHECK name.
 * $form['requirements']['GROUP']['CHECK'] = array(
 *   '#type' => 'container',
 *   '#tree' => TRUE,
 * );
 * $form['requirements']['GROUP']['CHECK']['active'] = array(
 *   '#type' => 'checkbox',
 *   '#return_value' => TRUE,
 *   '#title' => t('CHECK requirement statement'),
 *   '#default_value' => $config->get('requirements.GROUP.CHECK.active'),
 * );
 * @endcode
 *
 * You will also need to add default settings to
 * password_requirements.settings.json. Follow the format within the file to add
 * new groups and/or checks within existing groups.
 *
 * Functions for GROUP based requirements.
 */

/**
 * Check the CHECK.
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
function password_requirements_GROUP_CHECK_check($password, $username, array $config) {
  // Get the config into variables for ease of reference.
  $config_1 = $config['config_1'];
  // Define the check requirement text in a human readable statement including
  // relevant config as appropriate.
  $check_requirement = t('Passwords must contain @config_1.', array(
    '@config_1' => $config_1,
  ));
  // Complete the check, capturing intermediate steps as appropriate.
  $result = 'Carry out function, use preparatory steps for clarity if needed 
            and to reduce repetitive processing for the failure explanation';
  // If the check fails, prepare an explanation using the results.
  if (!$result) {
    $check_failure_explanation = t('Your password only has @result_part1.', array(
      '@result_part1' => $result_part1,
    ));
  }
  // Compile the output.
  return array(
    'result' => $result,
    'check_requirement' => $check_requirement,
    'check_failure_explanation' => $check_failure_explanation ?? '',
  );
}
