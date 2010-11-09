<?php
// $Id$

/**
 *  @file
 *  Create a Vimeo Stream Wrapper class for the Media/Resource module.
 */

/**
 *  Create an instance like this:
 *  $vimeo = new MediaVimeoStreamWrapper('vimeo://?v=[video-code]');
 */
class MediaVimeoStreamWrapper extends MediaReadOnlyStreamWrapper {
  protected $base_url = 'http://vimeo.com/';

  function getTarget($f) {
    return FALSE;
  }

  static function getMimeType($uri, $mapping = NULL) {
    return 'video/vimeo';
  }
}
