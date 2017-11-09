<?php

/**
 * @file
 * Utility functions used by the Islandora Object Clone module.
 */

/**
 * Ingests a new Islandora object based on the source object.
 *
 * We can't perform a dumb 'clone' of the object because the owner may be
 * different, and the createdDate will certainly be different. So, we will
 * need to copy the properties of the source object and ingest the new
 * object using them; we'll also need to iterate over the source object's
 * datastreams and ingest the new version of each of those as well.
 *
 * Some properties we'll need to populate:
 *
 * -label
 * -logMessage (e.g., cloned from....)
 * -models
 * -owner
 * -relationships
 * -state - but maybe this should be a user option?
 *
 * some datastream properties we'll need to populate:
 *
 * -checksumType
 * -content (via setContentFromString())
 * -controlGroup
 * -format (if it is set in the source)
 * -label
 * -logMessage (e.g., cloned from....)
 * -mimeType
 * -relationships (if set in the source)
 * -state
 * -url (if it is set in the source)
 * -versionable 
 *
 * We should not copy the RELS-EXT or RELS-INT since they should be rebuilt
 * using relationships.
 *
 * @param string $source_pid
 *
 * @return string $clone_pid
 */
function islandora_object_clone_clone_object($source_pid) {
  $source_object = islandora_object_load($source_pid);
  return $source_object->id;
}
