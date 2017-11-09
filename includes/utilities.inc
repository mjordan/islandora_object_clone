<?php

/**
 * @file
 * Utility functions used by the Islandora Object Clone module.
 */

/**
 * Ingests a new Islandora object based on the source object.
 *
 * @param string $source_pid
 *
 * @return string $clone_pid
 */
function islandora_object_clone_clone_object($source_pid) {
  $source_object = islandora_object_load($source_pid);
  return $source_object->id;
}
