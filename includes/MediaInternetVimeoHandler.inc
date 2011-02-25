<?php

/**
 * @file media_vimeo/includes/MediaInternetVimeoHandler.inc
 *
 * A definition of the MediaInternetVimeoHandler class for Media: Vimeo.
 */

class MediaInternetVimeoHandler extends MediaInternetBaseHandler {
  public function claim($embedCode) {
    if (media_vimeo_media_parse($embedCode)) {
      return TRUE;
    }
  }

  public function validate() {
    // @todo Media module currently fails when two files try to have the same
    //   URI, so catch that in the validation step. Some day, it would be nice
    //   to allow it, however. See http://drupal.org/node/952422.
    $uri = media_vimeo_media_parse($this->embedCode);
    $existing_files = file_load_multiple(array(), array('uri' => $uri));
    if (count($existing_files)) {
      throw new MediaInternetValidationException(t('You have entered a URL for a video that is already in your library.'));
    }
  }

  public function save() {
    $file = $this->getFileObject();
    file_save($file);
    return $file;
  }

  public function getFileObject() {
    $uri = media_vimeo_media_parse($this->embedCode);
    //@todo: this is terribly broken in some ways because the function is really
    // made for local files which are 'real'
    return file_uri_to_object($uri);
  }
}
