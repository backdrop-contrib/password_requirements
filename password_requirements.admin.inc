<?php
/**
 * @file
 *
 * Settings form for Password Requirements
 */

/**
 * Settings form
 *
 * @see system_settings_form()
 */
function password_requirements_settings() {
  $config = config('password_requirements.settings');
  $form = array();
  $form['#config'] = 'password_requirements.settings';
  $form['#validate'][] = 'password_requirements_settings_validate';
  $form['requirements'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#title' => t('Password Requirements'),
    '#description' => t('Select and configure each requirement you wish to set.'),
    '#collapsible' => FALSE,
  );
  // Characters.
  $form['requirements']['characters'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#title' => t('Characters'),
    '#description' => t('Define the types and number of characters to be included.'),
    '#collapsible' => TRUE,
  );
  // Character count.
  $form['requirements']['characters']['length'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
  );
  $form['requirements']['characters']['length']['active'] = array(
    '#type' => 'checkbox',
    '#return_value' => TRUE,
    '#title' => t('Require minimum number of characters in password'),
    '#default_value' => $config->get('requirements.characters.length.active'),
  );
  $form['requirements']['characters']['length']['count'] = array(
    '#type' => 'number',
    '#title' => t('Minimum length required'),
    '#min' => 1,
    '#default_value' => $config->get('requirements.characters.length.count'),
    '#states' => array(
      'visible' => array(
        ':input[name="requirements[characters][length][active]"]' => array('checked' => TRUE),
      ),
    ),
  );
  // Numbers.
  $form['requirements']['characters']['numbers'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
    '#title' => t('Numbers'),
    '#description' => t('Require numbers to be included.'),
  );
  $form['requirements']['characters']['numbers']['active'] = array(
    '#type' => 'checkbox',
    '#return_value' => TRUE,
    '#title' => t('Require numbers in password'),
    '#default_value' => $config->get('requirements.characters.numbers.active'),
  );
  $form['requirements']['characters']['numbers']['count'] = array(
    '#type' => 'number',
    '#title' => t('Count of numbers required'),
    '#min' => 1,
    '#default_value' => $config->get('requirements.characters.numbers.count'),
    '#states' => array(
      'visible' => array(
        ':input[name="requirements[characters][numbers][active]"]' => array('checked' => TRUE),
      ),
    ),
  );
  // Letters.
  $form['requirements']['characters']['letters'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
    '#title' => t('Letters'),
    '#title_display' => 'attribute',
    '#description' => t('Require letters to be included.'),
  );
  $form['requirements']['characters']['letters']['active'] = array(
    '#type' => 'checkbox',
    '#return_value' => TRUE,
    '#title' => t('Require letters in password'),
    '#default_value' => $config->get('requirements.characters.letters.active'),
  );
  $form['requirements']['characters']['letters']['count'] = array(
    '#type' => 'number',
    '#title' => t('Count of letters required'),
    '#min' => 1,
    '#default_value' => $config->get('requirements.characters.letters.count'),
    '#states' => array(
      'visible' => array(
        ':input[name="requirements[characters][letters][active]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['requirements']['characters']['letters']['mixed_case'] = array(
    '#type' => 'checkbox',
    '#return_value' => TRUE,
    '#title' => t('Require both upper and lower case letters in password'),
    '#default_value' => $config->get('requirements.characters.letters.mixed_case'),
    '#states' => array(
      'visible' => array(
        ':input[name="requirements[characters][letters][active]"]' => array('checked' => TRUE),
      ),
    ),
  );
  // Special characters.
  $form['requirements']['characters']['special_characters'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
    '#title' => t('Special characters'),
    '#description' => t('Require special characters to be included.'),
  );
  $form['requirements']['characters']['special_characters']['active'] = array(
    '#type' => 'checkbox',
    '#return_value' => TRUE,
    '#title' => t('Require special characters in password'),
    '#default_value' => $config->get('requirements.characters.special_characters.active'),
  );
  $form['requirements']['characters']['special_characters']['count'] = array(
    '#type' => 'number',
    '#title' => t('Count of special characters required'),
    '#min' => 1,
    '#default_value' => $config->get('requirements.characters.special_characters.count'),
    '#states' => array(
      'visible' => array(
        ':input[name="requirements[characters][special_characters][active]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['requirements']['characters']['special_characters']['character_set'] = array(
    '#type' => 'textfield',
    '#return_value' => TRUE,
    '#title' => t('Determine which characters are special characters'),
    '#default_value' => $config->get('requirements.characters.special_characters.character_set'),
    '#states' => array(
      'visible' => array(
        ':input[name="requirements[characters][special_characters][active]"]' => array('checked' => TRUE),
      ),
    ),
  );
  // Exclusions.
  $form['requirements']['exclusions'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#title' => t('Exclusions'),
    '#description' => t('Define and configure the types of word that must be excluded from the password.'),
    '#collapsible' => TRUE,
  );
  // Username.
  $form['requirements']['exclusions']['username'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
  );
  $form['requirements']['exclusions']['username']['active'] = array(
    '#type' => 'checkbox',
    '#return_value' => TRUE,
    '#title' => t('Password must not contain username.'),
    '#default_value' => $config->get('requirements.exclusions.username.active'),
  );
  // Past passwords.
  $form['requirements']['exclusions']['past_passwords'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
  );
  $form['requirements']['exclusions']['past_passwords']['active'] = array(
    '#type' => 'checkbox',
    '#return_value' => TRUE,
    '#title' => t('Prevent using past passwords.'),
    '#default_value' => $config->get('requirements.exclusions.past_passwords.active'),
  );
  $form['requirements']['exclusions']['past_passwords']['count'] = array(
    '#type' => 'number',
    '#title' => t('Count of previous passwords to check.'),
    '#min' => 1,
    '#default_value' => $config->get('requirements.exclusions.past_passwords.count'),
    '#description' => t('This check will not be active for the creation of user accounts, whether by the user registering or an account being added by a user administrator.'),
    '#states' => array(
      'visible' => array(
        ':input[name="requirements[exclusions][past_passwords][active]"]' => array('checked' => TRUE),
      ),
    ),
  );
  // Backdrop Strength.
  $form['requirements']['strength'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#title' => t('Password Strength'),
    '#description' => t('Require password to meet a selected password strength measure.'),
    '#collapsible' => TRUE,
  );
  $form['requirements']['strength']['backdrop_strength'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
  );
  $form['requirements']['strength']['backdrop_strength']['active'] = array(
    '#type' => 'checkbox',
    '#return_value' => TRUE,
    '#title' => t('Require password to meet the selected Backdrop strength level'),
    '#default_value' => $config->get('requirements.strength.backdrop_strength.active'),
  );
  $form['requirements']['strength']['backdrop_strength']['level'] = array(
    '#type' => 'radios',
    '#title' => t('Select level required'),
    '#options' => array(
      'USER_PASSWORD_STRENGTH_WEAK' => t('Weak'),
      'USER_PASSWORD_STRENGTH_FAIR' => t('Fair'),
      'USER_PASSWORD_STRENGTH_GOOD' => t('Good'),
      'USER_PASSWORD_STRENGTH_STRONG' => t('Strong'),
    ),
    '#default_value' => $config->get('requirements.strength.backdrop_strength.level'),
    '#states' => array(
      'visible' => array(
        ':input[name="requirements[strength][backdrop_strength][active]"]' => array('checked' => TRUE),
      ),
    ),
  );

  return system_settings_form($form);
}

/**
 * Validation handler for Password Requirements settings form.
 */
function password_requirements_settings_validate($form, &$form_state) {
  // Set the top level of the characters array for ease of reference.
  $values = $form_state['values']['requirements']['characters'];
  // Check that the sum of character type counts <= character length.
  if ($values['length']['active']) {
    $length = $values['length']['count'];
    $numbers = $values['numbers']['count'] * (int) $values['numbers']['active'];
    $letters = $values['letters']['count'] * (int) $values['letters']['active'];
    $special_characters = $values['special_characters']['count'] * (int) $values['special_characters']['active'];
    $sum_character_types = $numbers + $letters + $special_characters;
    $check = $sum_character_types <= $length;
    if (!$check) {
      $error_message = t("The minimum length required is less than the sum of character type(s) required.");
      form_set_error('requirements][characters][length][count', $error_message);
    }
  }
  // Check if mixed case letters are required that the letters count >= 2.
  if ($values['letters']['active']) {
    $letters = $values['letters']['count'];
    $mixed_case = $values['letters']['mixed_case'];
    if ($mixed_case) {
      $check = $letters >= 2;
      if (!$check) {
        $error_message = t("If mixed case letters are required then the count of letters must be at least 2.");
        form_set_error('requirements][characters][letters][count', $error_message);
      }
    }
  }
}
