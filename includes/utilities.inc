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
 * @param array $form_state
 *   The $form_state from the submitted confirmation form.
 *
 * @return string
 *   The PID of the cloned object.
 */
function islandora_object_clone_clone_object($form_state) {
  $source_pid = $form_state['values']['islandora_object_clone_source_pid'];
  $confirmed_namespace = $form_state['values']['islandora_object_clone_confirm_namespace'];
  $confirmed_label = trim($form_state['values']['islandora_object_clone_confirm_label']);
  $confirmed_state = trim($form_state['values']['islandora_object_clone_confirm_state']);

  $source_object = islandora_object_load($source_pid);

  $tuque = islandora_get_tuque_connection();
  $repository = $tuque->repository;
  $obj = $repository->constructObject($confirmed_namespace);

  global $user;
  $obj->owner = $user->name;
  $obj->label = $confirmed_label;
  $obj->state = $confirmed_state;

  // Let's talk relationships. Copy them all from the source to the
  // cloned object.
  foreach ($source_object->relationships->get() as $rel) {
    $obj->relationships->add(
      $rel['predicate']['namespace'],
      $rel['predicate']['value'],
      $rel['object']['value']
    );
  }

  // Datastreams.
  $cloned_datastreams = islandora_object_clone_datastreams($source_object);
  foreach ($cloned_datastreams as $ds) {
    $obj->ingestDatastream($ds);
  }

  $new_object = $repository->ingestObject($obj);
  return $new_object->id;
}


/**
 * Clones the source object's datastreams.
 *
 * @param object $source_object
 *   The source object.
 *
 * @return array
 *   A list of all the cloned datastreams to add to the cloned object.
 */
function islandora_object_clone_datastreams($source_object) {
  $skip_these = array('RELS-EXT');
  $cloned_datastreams = array();
  foreach ($source_object as $source_ds) {
    if (!in_array($source_ds->id, $skip_these)) {
      $ds = $source_object->constructDatastream($source_ds->id, $source_ds->controlGroup);
      $ds->label = $source_ds->label;
      $ds->setContentFromString($source_ds->content);
      $ds->mimetype = $source_ds->mimetype;
      $ds->checksumType = $source_ds->checksumType;
      $ds->versionable = $source_ds->versionable;
      $ds->state = $source_ds->state;
      $cloned_datastreams[] = $ds;
    }
  }

  return $cloned_datastreams;
}
