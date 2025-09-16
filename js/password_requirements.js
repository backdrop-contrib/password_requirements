(function ($) {
  "use strict";
  /**
   * Attach handlers to evaluate compliance of password fields with password
   * requirements and provide feedback.
   */
  Backdrop.behaviors.PasswordRequirementsCheck = {
    attach: function (context, settings) {
      $('input.password-field', context).once('password-requirements-check', function () {

        // Create password check dom elements and apply them around the password field.
        var $self = $(this),
            $container = $('<div class="password-requirements-check"></div>'),
            $strength_bar = $('<div class="password-requirements strength-bar"><div class="bar"><div class="value"></div></div></div>'),
            $message_container = $('<div class="password-requirements message-container"></div>'),
            $message_feedback = $('<div class="password-requirements check-feedback"></div>');
        $self.wrap($container);
        $self.after($message_feedback);
        $("div[class^='password-requirements check-feedback']").wrapAll($message_container);

        // Hide the message elements.
        $message_feedback.hide();

        var passwordCheck = function (e, isCallback) {
          if (typeof isCallback != 'undefined') {
            return;
          }

          e.stopImmediatePropagation();
          // Define the request to the AJAX check.
          var request_data = {
            password: encodeURIComponent($self.val()),
            token: Backdrop.settings.passwordRequirements.token,
            uid: Backdrop.settings.passwordRequirements.uid
          };
          // Set username value as parameter if it is present.
          var usernameInput = $('input.username');
          var username = usernameInput.val();
          if (username) {
            request_data['username'] = encodeURIComponent(username);
          }

          console.log(request_data);
          // Check password for compliance with the requirements.
          $.post(
            Backdrop.settings.passwordRequirements.secure_base_url + 'ajax/people/password-requirements/check',
            request_data,
            function(data) {

              // Password field is empty.
              var is_empty = (!$self.val());

              // Set message content.
              $message_feedback.html(data.check_output);

              // Show the feedback message.
              if ($message_feedback.is(':hidden')) {
                $message_feedback.slideDown();
              }
              // Hide the feedback is password is empty.
              else if (is_empty && $message_feedback.is(':visible')) {
                $message_feedback.slideUp();
              }
            }
          );

        };

        var position = function () {
          // Position the strength meter inside of the password field and adjust
          var width = $self.outerWidth(),
              height = $self.outerHeight(),
              bar_height = $strength_bar.outerHeight();
          $strength_bar.css({ width: width - 2, left: 1, top: height - bar_height });
          $message_feedback.css({ width: width });
        };

        // Reposition the element after transitioning.
        $(window).bind('resize transitionend', function(){
          position();
        });

        // Prevent evaluating password right away on each keystroke, instead wait
        // for a bit and send the updated password in less frequent batches.
        $self.bindWithDelay('keyup focusin', passwordCheck, 500, true);

        // Trigger the passwordCheck right away when js initializes if value is not empty.
        if ($self.val()) {
          $self.trigger('focusin');
        }

        // Position the elements on the page.
        position();

      });
    }
  };

  /**
   * Define an alternative to bind function that will delay execution.
   */
  $.fn.bindWithDelay = function(type, data, fn, timeout, throttle) {

    if ( $.isFunction( data ) ) {
      throttle = timeout;
      timeout = fn;
      fn = data;
      data = undefined;
    }

    // Allow delayed function to be removed with fn in unbind function
    fn.guid = fn.guid || ($.guid && $.guid++);

    // Bind each separately so that each element has its own delay
    return this.each(function() {
      var wait = null;

      function cb() {
        var e = $.extend(true, { }, arguments[0]);
        var ctx = this;
        var throttler = function() {
          wait = null;
          fn.apply(ctx, [e]);
        };

        if (!throttle) { clearTimeout(wait); wait = null; }
        if (!wait) { wait = setTimeout(throttler, timeout); }
      }

      cb.guid = fn.guid;

      $(this).bind(type, data, cb);
    });

  };

  // Parse query string from URL.
  var queryString = (function(a) {
    if (a == "") return {};
    var b = {};
    for (var i = 0; i < a.length; ++i) {
      var p=a[i].split('=');
      if (p.length != 2) continue;
      b[p[0]] = decodeURIComponent(p[1].replace(/\+/g, " "));
    }
    return b;
  })(window.location.search.substring(1).split('&'))

})(jQuery);
