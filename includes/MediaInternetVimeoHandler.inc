<?php

/**
 * @file
 * Extends the MediaInternetBaseHandler class to handle Vimeo videos.
 */

/**
 * Implementation of MediaInternetBaseHandler.
 *
 * @see hook_media_internet_providers().
 */
class MediaInternetVimeoHandler extends MediaInternetBaseHandler {
  public function parse($embedCode) {
    // http://vimeo.com/*
    // http://vimeo.com/video/*
    // http://vimeo.com/groups/*/videos/*
    // http://vimeo.com/channels/*#$ID
    $patterns = array(
      '@vimeo\.com/(\d+)@i',
      '@vimeo\.com/video/(\d+)@i',
      '@vimeo\.com/groups/.+/videos/(\d+)@i',
      '@vimeo\.com/channels/.+#(\d+)@i',
    );

    foreach ($patterns as $pattern) {
      preg_match($pattern, $embedCode, $matches);
      if (isset($matches[1])) {
        return file_stream_wrapper_uri_normalize('vimeo://v/' . $matches[1]);
      }
    }
  }

  public function claim($embedCode) {
    if ($this->parse($embedCode)) {
      return TRUE;
    }
  }

  public function getFileObject() {
    $uri = $this->parse($this->embedCode);
    $file = file_uri_to_object($uri, TRUE);

    // Try to default the file name to the video's title.
    if (empty($file->fid) && $info = $this->getOEmbed()) {
      $file->filename = truncate_utf8($info['title'], 255);
    }

    return $file;
  }

  /**
   * Returns information about the media.
   *
   * See http://www.oembed.com.
   *
   * @return
   *   If oEmbed information is available, an array containing 'title', 'type',
   *   'url', and other information as specified by the oEmbed standard.
   *   Otherwise, NULL.
   */
  public function getOEmbed() {
    $uri = $this->parse($this->embedCode);
    $external_url = drupal_realpath($uri);
    $oembed_url = url('http://vimeo.com/api/oembed.json', array('query' => array('url' => $external_url)));
    $response = drupal_http_request($oembed_url);

    if (!isset($response->error)) {
      return drupal_json_decode($response->data);
    }
    else {
      throw new Exception("Error Processing Request. (Error: {$response->code}, {$response->error})");
      return;
    }
  }
}
