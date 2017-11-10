<?php
/**
 * @file
 * This file documents hook functions for the Islandora Object Clone module.
 */

/**
 * Allow datastreams created during the cloning process to be altered.
 *
 * @param AbstractObject $new_ds
 *   The newly constructed datastream, after it has had its properties
 *   added based on those of the source datastream, and just before it
 *   is ingested into the new object.
 * @param AbstractObject $source_object
 *   The source object.
 */
function hook_islandora_object_clone_newds_alter(&$new_ds, $source_object) {
  if ($new_ds->id == 'MODS') {
    $new_ds->label = 'MODS is rad';
  }
}
